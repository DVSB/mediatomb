/*  Mysql_storage.cc - this file is part of MediaTomb.

    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
                       Sergey Bostandzhyan <jin@deadlock.dhs.org>

    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifdef HAVE_MYSQL

#include "mysql_storage.h"
#include "config_manager.h"
#include "destroyer.h"

using namespace zmm;
using namespace mxml;

MysqlStorage::MysqlStorage() : SQLStorage()
{
    db = NULL;
    int res;
    
    pthread_mutex_init(&lock_mutex, NULL);    
    /*
    pthread_mutexattr_t mutex_attr;
    res = pthread_mutexattr_init(&mutex_attr);
    res = pthread_mutexattr_settype(&mutex_attr, PTHREAD_MUTEX_RECURSIVE_NP);
    pthread_mutex_init(&lock_mutex, &mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    
    Recursive mutex is apparently not needed anymore
    */
    
}
MysqlStorage::~MysqlStorage()
{
    if (db)
    {
        mysql_close(db);
        free(db);
    }
    pthread_mutex_destroy(&lock_mutex);
}

void MysqlStorage::init()
{
    Ref<ConfigManager> config = ConfigManager::getInstance();

    String dbHost = config->getOption("/server/storage/host");
    String dbName = config->getOption("/server/storage/database");
    String dbUser = config->getOption("/server/storage/username");
    String dbPass = config->getOption("/server/storage/password", "");

    db = (MYSQL *)malloc(sizeof(MYSQL));    
    
    MYSQL *res_mysql;

    res_mysql = mysql_init(db);
    if(! res_mysql)
    {
        free(db);
        db = NULL;
        reportError(nil);
        throw StorageException("MysqlStorage.init: mysql_init() failed");
    }

    res_mysql = mysql_real_connect(db,
        dbHost.c_str(),
        dbUser.c_str(),
        dbPass.c_str(),
        dbName.c_str(),
        0, // port
        NULL, // socket
        0 // flags
    );
    if(! res_mysql)
    {
        mysql_close(db);
        free(db);
        db = NULL;
        reportError(nil);
        throw StorageException("MysqlStorage.init: mysql_real_connect() failed");
    }
}

String MysqlStorage::quote(String value)
{
    char *q = (char *)malloc(value.length() * 2);
    *q = '\'';
    int size = mysql_real_escape_string(db, q + 1, value.c_str(), value.length());
    q[size + 1] = '\'';
    String ret(q, size + 2);
    free(q);
    return ret;
}

void MysqlStorage::reportError(String query)
{
    if (query == nil)
        query = "unknown";
    fprintf(stderr, "Mysql: (%d) %s\nQuery:%s\n",
        mysql_errno(db),
        mysql_error(db),
        query.c_str());
}


Ref<SQLResult> MysqlStorage::select(String query)
{ 
    lock();
    
    int res;
    res = mysql_real_query(db, query.c_str(), query.length());
    if(res)
    {
        unlock();
        reportError(query);
        throw StorageException("Mysql: mysql_real_query() failed");
    }

    MYSQL_RES *mysql_res;
    mysql_res = mysql_store_result(db);
    if(! mysql_res)
    {
        unlock();
        reportError(query);
        throw StorageException("Mysql: mysql_store_result() failed");
    }
    Ref<SQLResult> ret(new MysqlResult(mysql_res));    

    unlock();
    
    return ret;
}

int MysqlStorage::exec(String query)
{
    lock();
    
    int res;
    res = mysql_real_query(db, query.c_str(), query.length());
    if(res)
    {
        reportError(query);
        unlock();
        throw StorageException("Mysql: query error");
    }
    unlock();
}

int MysqlStorage::lastInsertID()
{
    return mysql_insert_id(db);    
}

void MysqlStorage::lock()
{
    pthread_mutex_lock(&lock_mutex);
}
void MysqlStorage::unlock()
{
    pthread_mutex_unlock(&lock_mutex);
}

/* MysqlResult */


MysqlResult::MysqlResult(MYSQL_RES *mysql_res) : SQLResult()
{
    this->mysql_res = mysql_res;
    nullRead = false;
}
MysqlResult::~MysqlResult()
{
    if(mysql_res)
    {
        if (! nullRead)
        {
            MYSQL_ROW mysql_row;
            while(mysql_row = mysql_fetch_row(mysql_res)); // read out data
        }
        mysql_free_result(mysql_res);
        mysql_res = NULL;        
    }
}
Ref<SQLRow> MysqlResult::nextRow()
{   
    MYSQL_ROW mysql_row;
    mysql_row = mysql_fetch_row(mysql_res);
    if(mysql_row)
    {
        return Ref<SQLRow>(new MysqlRow(mysql_row));
    }
    nullRead = true;
    mysql_free_result(mysql_res);
    mysql_res = NULL;
    return nil;
}


/* MysqlRow */

MysqlRow::MysqlRow(MYSQL_ROW mysql_row)
{
    this->mysql_row = mysql_row;
}
String MysqlRow::col(int index)
{
    return String(mysql_row[index]);
}

#endif // HAVE_MYSQL
