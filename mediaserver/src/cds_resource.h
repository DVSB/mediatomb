/*  cds_resource.h - this file is part of MediaTomb.
                                                                                
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

#ifndef __CDS_RESOURCE_H__
#define __CDS_RESOURCE_H__

#include "common.h"
#include "dictionary.h"

#define RESOURCE_SEP '|'
#define RESOURCE_PART_SEP '~'

class CdsResource : public zmm::Object
{
protected:
    int handlerType;
    zmm::Ref<Dictionary> attributes;
    zmm::Ref<Dictionary> parameters;

public:
    CdsResource(int handlerType);
    CdsResource(int handlerType,
                zmm::Ref<Dictionary> attributes,
                zmm::Ref<Dictionary> parameters);
    
    void addAttribute(zmm::String name, zmm::String value);
    void addParameter(zmm::String name, zmm::String value);

    // urlencode into string
    int getHandlerType();
    zmm::Ref<Dictionary> getAttributes();
    zmm::Ref<Dictionary> getParameters();
    zmm::String getAttribute(zmm::String name);
    zmm::String getParameter(zmm::String name);

    bool equals(zmm::Ref<CdsResource> other);
    zmm::Ref<CdsResource> clone();

    zmm::String encode();
    static zmm::Ref<CdsResource> decode(zmm::String serial);

};

#endif // __CDS_RESOURCE_H__
