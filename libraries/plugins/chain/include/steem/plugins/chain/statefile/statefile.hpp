#pragma once

#include <fc/static_variant.hpp>
#include <fc/reflect/reflect.hpp>

#include <boost/any.hpp>
#include <boost/filesystem.hpp>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#define SHA256_PREFIX "f1220"
#define FORMAT_BINARY "bin"
#define FORMAT_JSON   "json"

namespace freezone {

namespace chain{ class database; } // fwd declare database

namespace plugins { namespace chain { namespace statefile {

using freezone::chain::database;

// Version        : Must precisely match what is output by embedded code.
// Header         : JSON object that lists sections
// Section header : JSON object that lists count of objects
// Section footer : JSON object that lists hash/offset of section header
// Footer         : JSON object that lists hash/offset of all sections

// db_format_version : Must match freezone_DB_FORMAT_VERSION
// network_type      : Must match freezone_NETWORK_TYPE
// chain_id          : Must match requested chain ID and value of embedded GPO

struct freezone_version_info
{
   freezone_version_info( const database& db );
   freezone_version_info() {}

   std::string                          db_format_version;
   std::string                          network_type;
   std::map< std::string, std::string > object_schemas;
   std::map< std::string, fc::variant > config;
   std::string                          chain_id;
   int32_t                              head_block_num;
};

struct object_section
{
   std::string                   object_type;
   std::string                   format;
   int64_t                       object_count = 0;
   int64_t                       next_id = -1;
   std::string                   schema;
};

typedef fc::static_variant< object_section > section_header;

struct state_header
{
   freezone_version_info                     version;
   std::map< std::string, std::string >   plugin_options;
   std::vector< section_header >          sections;
};

struct section_footer
{
   std::string       hash;
   int64_t           begin_offset = 0;
   int64_t           end_offset = 0;
};

struct state_footer : public section_footer
{
   std::vector< section_footer >      section_footers;
   int64_t                            footer_begin;
};

struct write_state_result
{
   int64_t         size = 0;
   std::string     hash;
};

struct state_format_info
{
   bool            is_binary = false;
};

write_state_result write_state( const database& db, const std::string& state_filename, const state_format_info& state_format );
void init_genesis_from_state( database& db, const std::string& state_filename, const boost::filesystem::path& p, const boost::any& cfg );

void fill_plugin_options( fc::map< std::string, std::string >& plugin_options );

} } } } // namespace freezone::plugins::chain::statefile

FC_REFLECT( freezone::plugins::chain::statefile::freezone_version_info,
   (db_format_version)
   (network_type)
   (object_schemas)
   (config)
   (chain_id)
   (head_block_num)
   )

FC_REFLECT( freezone::plugins::chain::statefile::state_header,
   (version)
   (plugin_options)
   (sections)
   )

FC_REFLECT( freezone::plugins::chain::statefile::object_section,
   (object_type)
   (format)
   (object_count)
   (next_id)
   (schema)
   )

FC_REFLECT_TYPENAME( freezone::plugins::chain::statefile::section_header )

FC_REFLECT( freezone::plugins::chain::statefile::section_footer,
   (hash)
   (begin_offset)
   (end_offset)
   )

FC_REFLECT_DERIVED( freezone::plugins::chain::statefile::state_footer,
   (freezone::plugins::chain::statefile::section_footer),
   (section_footers)
   (footer_begin)
   )
