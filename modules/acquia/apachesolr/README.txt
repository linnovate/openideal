/* $Id: README.txt,v 1.1.2.1.2.36 2010/07/25 16:16:46 pwolanin Exp $ */

This module integrates Drupal with the Apache Solr search platform. Solr search
can be used as a replacement for core content search and boasts both extra
features and better performance. Among the extra features is the ability to have
faceted search on facets ranging from content author to taxonomy to arbitrary
CCK fields.

The module comes with a schema.xml and solrconfig.xml file which should be used
in your Solr installation.

This module depends on the search framework in core. However, you may not want
the core searches and only want Solr search. If that is the case, you want to
use the Core Searches module in tandem with this module.

When used in combination with core search module, Apache Solr is not the default
search. Access it via a new tab on the default search page, called "Search".

Installation
------------

Prerequisite: Java 5 or higher (a.k.a. 1.5.x).  PHP 5.1.4 or higher.

Those with PHP < 5.2.0 must install the PECL json module or download
the Json code from the Zend Framework (see below).

Install the Apache Solr Drupal module as you would any Drupal module.

Before enabling it, you must also do the following:

Get the PHP library from the external project. The project is
found at:  http://code.google.com/p/solr-php-client/
From the apachesolr module directory, run this command:

svn checkout -r22 http://solr-php-client.googlecode.com/svn/trunk/ SolrPhpClient

Alternately you may download and extract the library from
http://code.google.com/p/solr-php-client/downloads/list

Make sure to select a r22 archive, either of these two:
http://solr-php-client.googlecode.com/files/SolrPhpClient.r22.2009-11-09.zip
http://solr-php-client.googlecode.com/files/SolrPhpClient.r22.2009-11-09.tgz

Note that revision 22 is the currently tested and required revision. 
Make sure that the final directory is named SolrPhpClient under the apachesolr
module directory.  

If you are maintaing your code base in subversion, you may choose instead to 
use svn export or svn externals. For an export (writing a copy to your local
directory without .svn files to track changes) use:

svn export -r22 http://solr-php-client.googlecode.com/svn/trunk/ SolrPhpClient

Instead of checking out, externals can be used too. Externals can be seen as 
(remote) symlinks in svn. This requires your own project in your own svn ]
repository, off course. In the apachesolr module directory, issue the command:

svn propedit svn:externals .

Your editor will open. Add a line

SolrPhpClient -r22 http://solr-php-client.googlecode.com/svn/trunk/

On exports and checkouts, svn will grab the externals, but it will keep the 
references on the remote server.

Those without svn, etc may also choose to try the bundled Acquia Search
download, which includes all the items which are not in Drupal.org CVS due to 
CVS use policy. See the download link here: 
http://acquia.com/documentation/acquia-search/activation

Download Solr 1.4 from:
http://www.apache.org/dyn/closer.cgi/lucene/solr/

Unpack the tarball somewhere not visible to the web (not in your apache docroot
and not inside of your drupal directory).

The Solr download comes with an example application that you can use for
testing, development, and even for smaller production sites. This
application is found at apache-solr-nightly/example.

Move apache-solr-nightly/example/solr/conf/schema.xml and rename it to
something like schema.bak. Then move the schema.xml that comes with the
ApacheSolr Drupal module to take its place.

Similarly, move apache-solr-nightly/example/solr/conf/solrconfig.xml and rename
it like solrconfig.bak. Then move the solrconfig.xml that comes with the
ApacheSolr Drupal module to take its place.

Now start the solr application by opening a shell, changing directory to
apache-solr-nightly/example, and executing the command java -jar start.jar

Test that your solr server is now available by visiting
http://localhost:8983/solr/admin/

For those using PHP 5.1, you must either install the PECL json extension
into PHP on your sever, or you may use the Zend framework Json library.
for the PECL extension see:  http://pecl.php.net/package/json
The Solr client has been tested with Zend framework release 1.7.7.
To get this code, you may use svn from the apachesolr directory:
svn co http://framework.zend.com/svn/framework/standard/tags/release-1.7.7/library/Zend
However, the only required parts are:
http://framework.zend.com/svn/framework/standard/tags/release-1.7.7/library/Zend/Exception.php
http://framework.zend.com/svn/framework/standard/tags/release-1.7.7/library/Zend/Json/
The 'Zend' directory should normally be under the apachesolr
directory, but may be elsewhere if you set that location to be
in your PHP include path.

Now, you should enable the "Apache Solr framework" and "Apache Solr search" 
modules. Check that you can connect to Solr at ?q=admin/setting/apachesolr
Now run cron on your Drupal site until your content is indexed. You
can monitor the index at ?q=admin/settings/apachesolr/index

The solrconfig.xml that comes with this modules defines auto-commit, so
it may take a few minutes between running cron and when the new content
is visible in search.

Enable blocks for facets first at Administer > Site configuration > Apache Solr > Enabled filters,
then position them as you like at Administer > Site building > Blocks.   

Configuration variables
--------------

The module provides some (hidden) variables that can be used to tweak its
behavior:

 - apachesolr_luke_limit: the limit (in terms of number of documents in the
   index) above which the module will not retrieve the number of terms per field
   when performing LUKE queries (for performance reasons).

 - apachesolr_tags_to_index: the list of HTML tags that the module will index
   (see apachesolr_add_tags_to_document()).

 - apachesolr_exclude_comments_types: an array of node types.  Any type listed
   will have any attached comments excluded from the index.

 - apachesolr_ping_timeout: the timeout (in seconds) after which the module will
   consider the Apache Solr server unavailable.

 - apachesolr_optimize_interval: the interval (in seconds) between automatic
   optimizations of the Apache Solr index. Set to 0 to disable.

 - apachesolr_cache_delay: the interval (in seconds) after an update after which
   the module will requery the Apache Solr for the index structure. Set it to
   your autocommit delay plus a few seconds.

 - apachesolr_service_class: the Apache_Solr_Service class used for communicating
   with the Apache Solr server.

 - apachesolr_query_class: the default query class to use.

 - apachesolr_cron_mass_limit: update or delete at most this many documents in
   each Solr request, such as when making {apachesolr_search_node} consistent
   with {node}.

Troubleshooting
--------------
Problem:
Links to nodes appear in the search results with a different host name or
subdomain than is preferred.  e.g. sometimes at http://example.com
and sometimes at http://www.example.com

Solution:
Set $base_url in settings.php to insure that an identical absolute url is
generated at all times when nodes are indexed.  Alternately, set up a re-direct
in .htaccess to prevent site visitors from accessing the site via more than one
site address.


Developers
--------------

Exposed Hooks in 6.x:

hook_apachesolr_modify_query(&$query, &$params, $caller);

  Any module performing a search should call apachesolr_modify_query($query, $params, 'modulename'). 
  That function then invokes this hook. It allows modules to modify the query object and params array. 
  $caller indicates which module is invoking the hook.

  Example:

        function my_module_apachesolr_modify_query(&$query, &$params, $caller) {
          // I only want to see articles by the admin!
          $query->add_filter("uid", 1);         
        }        

CALLER_finalize_query(&$query, &$params);

  The module calling apachesolr_do_query() may implement a function that is run after
  hook_apachesolr_modify_query() and allows the caller to make final changes to the
  query and params before the query is sent to Solr.  The function name is built
  from the $caller parameter to apachesolr_do_query().

hook_apachesolr_prepare_query(&$query, &$params, $caller);

  This is pretty much the same as hook_apachesolr_modify_query() but runs earlier
  and before the query is statically cached. It can e.g. be used to add
  available sorts to the query.

  Example:

        function my_module_apachesolr_prepare_query(&$query) {
          // Add a sort on the node ID.
          $query->set_available_sort('nid', array(
            'title' => t('Node ID'),
            'default' => 'asc',
          ));
        }

hook_apachesolr_cck_fields_alter(&$mappings)

  Add or alter index mappings for CCK types.  The default mappings array handles just 
  text fields with option widgets:

    $mappings['text'] = array(
      'optionwidgets_select' => array('callback' => '', 'index_type' => 'string'),
      'optionwidgets_buttons' => array('callback' => '', 'index_type' => 'string')
    );

  In your _alter hook implementation you can add additional field types such as:

    $mappings['number_integer']['number'] = array('callback' => '', 'index_type' => 'integer');

  You can allso add a mapping for a specific field.  This will take precedence over any
  mapping for a general field type. A field-specific mapping would look like:

    $mappings['per-field']['field_model_name'] = array('callback' => '', 'index_type' => 'string');

  or

    $mappings['per-field']['field_model_price'] = array('callback' => '', 'index_type' => 'float');

hook_apachesolr_types_exclude($namespace)

  
  Invoked by apachesolr.module when generating a list of nodes to index for a given
  namespace.  Return an array of node types to be excldued from indexing for that namespace 
  (e.g. 'apachesolr_search'). This is used by apachesolr_search module to exclude 
  certain node types from the index.

hook_apachesolr_node_exclude($node, $namespace)

  This is invoked by apachesolr.module for each node to be added to the index - if any module
  returns TRUE, the node is skipped for indexing. 

hook_apachesolr_update_index(&$document, $node)

  Allows a module to change the contents of the $document object before it is sent to the Solr Server.
  To add a new field to the document, you should generally use one of the pre-defined dynamic fields. 
  Follow the naming conventions for the type of data being added based on the schema.xml file.

hook_apachesolr_search_result_alter(&$doc)

  The is invoked by apachesolr_search.module for each document returned in a search - new in 6.x-beta7
  as a replacement for the call to hook_nodeapi().

hook_apachesolr_sort_links_alter(&$sort_links)

  Called by the sort link block code. Allows other modules to modify, add or remove sorts.

Themers
----------------

See inline docs in apachesolr_theme and apachesolr_search_theme functions 
within apachesolr.module and apachesolr_search.module.

