// Copyright 2009-2010 Sandia Corporation. Under the terms
// of Contract DE-AC04-94AL85000 with Sandia Corporation, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2010, Sandia Corporation
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#ifndef SST_SYNC_H
#define SST_SYNC_H
#include <map>

#include "sst/core/sst.h"
#include "sst/core/action.h"
#include "sst/core/timeConverter.h"
#include "sst/core/syncQueue.h"

namespace SST {

#define _SYNC_DBG( fmt, args...) __DBG( DBG_SYNC, Sync, fmt, ## args )

class SyncQueue;
class Link;
class TimeConverter;

class Sync : public Action {
public:
    Sync(TimeConverter* period);
    ~Sync();

    SyncQueue* registerLink(int rank, LinkId_t link_id, Link* link);
    void execute(void);
    
private:
    typedef std::map<int, std::pair<SyncQueue*, std::vector<Activity*>* > > comm_map_t;
    typedef std::map<LinkId_t, Link*> link_map_t;

    Sync();

    TimeConverter* period;
    comm_map_t comm_map;
    link_map_t link_map;
    boost::mpi::communicator comm;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        printf("begin Sync::serialize\n");
        ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Action);
        ar & BOOST_SERIALIZATION_NVP(period);
        ar & BOOST_SERIALIZATION_NVP(comm_map);
        ar & BOOST_SERIALIZATION_NVP(link_map);
        // don't serialize comm - let it be silently rebuilt at restart
        printf("end Sync::serialize\n");
    }
};

} // namespace SST

#endif // SST_SYNC_H
