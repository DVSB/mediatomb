/*MT*
    
    MediaTomb - http://www.mediatomb.cc/
    
    flac_handler.h - this file is part of MediaTomb.
    
    Copyright (C) 2005 Gena Batyan <bgeradz@mediatomb.cc>,
                       Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>
    
    Copyright (C) 2006-2009 Gena Batyan <bgeradz@mediatomb.cc>,
                            Sergey 'Jin' Bostandzhyan <jin@mediatomb.cc>,
                            Leonhard Wimmer <leo@mediatomb.cc>
    
    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 2
    as published by the Free Software Foundation.
    
    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    version 2 along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.
    
    $Id: flac_handler.h 2010 2009-01-11 19:10:43Z lww $
*/

/// \file flac_handler.h
/// \brief Definition of the FlacHandler class.

#ifndef __METADATA_FLAC_H__
#define __METADATA_FLAC_H__

#include "metadata_handler.h"

/// \brief This class is responsible for reading FLAC metadata
class FlacHandler : public MetadataHandler
{
public:
    FlacHandler();
    virtual void fillMetadata(zmm::Ref<CdsItem> item);
    virtual zmm::Ref<IOHandler> serveContent(zmm::Ref<CdsItem> item, int resNum,
                                             off_t *data_size);
};

#endif // __METADATA_FLAC_H__
