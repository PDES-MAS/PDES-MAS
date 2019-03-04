/*
 * Header file that maintains the route from one CLP Node to another on the tree 
 * 
 * Date: 21st March, 2005
 */

#ifndef ROUTINGINFO_H
#define ROUTINGINFO_H

#include <iostream>

using namespace std;

namespace pdesmas {
  class RoutingInfo {
    private:
      unsigned int fNodeRank; // The current CLP node
      unsigned int fDestination; // The Destination Node
      unsigned int fNumberOfHops; // Number of Hops to the Destination Node
      unsigned int fNextNodeRank; // The Next Node towards that destination

    public:
      RoutingInfo();
      RoutingInfo(unsigned int, unsigned int, unsigned int, unsigned int);
      RoutingInfo(const RoutingInfo&);
      ~RoutingInfo();

      unsigned int GetNodeRank() const;
      unsigned int GetDestination() const;
      unsigned int GetHops() const;
      unsigned int GetNextNodeRank() const;
  };
}

#endif // ROUTINGTABLE_H
