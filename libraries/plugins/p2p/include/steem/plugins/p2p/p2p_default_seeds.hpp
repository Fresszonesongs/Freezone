#pragma once

#include <vector>

namespace freezone{ namespace plugins { namespace p2p {

#ifdef IS_TEST_NET
const std::vector< std::string > default_seeds;
#else
const std::vector< std::string > default_seeds = {
   "seed-east.freezone.com:2001",          // freezone
   "seed-central.freezone.com:2001",       // freezone
   "seed-west.freezone.com:2001",          // freezone
   "52.74.152.79:2001",                   // smooth.witness
   "anyx.io:2001",                        // anyx
   "seed.liondani.com:2016",              // liondani
   "gtg.freezone.house:2001",                // gtg
   "seed.jesta.us:2001",                  // jesta
   "lafonafreezone.com:2001",                // lafona
   "freezone-seed.altcap.io:40696",          // ihashfury
   "seed.roelandp.nl:2001",               // roelandp
   "seed.timcliff.com:2001",              // timcliff
   "freezoneseed.clayop.com:2001",           // clayop
   "seed.freezoneviz.com:2001",              // ausbitbank
   "freezone-seed.lukestokes.info:2001",     // lukestokes
   "seed.freezoneian.info:2001",             // drakos
   "seed.followbtcnews.com:2001",         // followbtcnews
   "node.mahdiyari.info:2001",            // mahdiyari
   "seed.riverfreezone.com:2001",            // riverhead
   "seed1.blockbrothers.io:2001",         // blockbrothers
   "freezoneseed-fin.privex.io:2001",        // privex
   "seed.yabapmatt.com:2001"              // yabapmatt
};
#endif

} } } // freezone::plugins::p2p
