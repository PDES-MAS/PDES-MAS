/* A quasi routing table for the load balancing algorithm  
 * Actually this is a forwarding table
 *
 * Date: 21st March, 2005

 * Modified by chen dan: 23~30/03/2005
 * Change the table from constant size (3) to variable
 * Add some helper methods in dealing with dynamic 2 dimension array
 */

#ifndef FORWARDINGTABLE_H
#define FORWARDINGTABLE_H

#include <iostream>
#include <cstring>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include "RoutingInfo.h"

using namespace std;

namespace pdesmas {
  class ForwardingTable {
    private:
      unsigned int fClpRank;
      unsigned int fNumberOfClps;
      map<unsigned int, RoutingInfo> fForwardingTable; // State Variable forwarding Table

      void WalkTree(unsigned int, unsigned int, unsigned int);
    public:
      ForwardingTable(unsigned int, unsigned int);
      ~ForwardingTable();

      const RoutingInfo& GetRoutingInfo(unsigned int) const;
  };
}
#endif // FORWARDINGTABLE_H
