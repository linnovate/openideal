Admin 2.x
=========
The admin module provides UI improvements to the standard Drupal admin interface. The 2.x branch focuses on the following goals:

1. Sustainability - avoid excessive overrides of code, markup, and
   interface strings to ensure the module keeps the workload overhead
   on the maintainers and community to a minimum.

2. Pluggable/extensible architecture - ensure that admin serves as a
   starting point for other modules in contrib to implement admin
   interfaces.

3. Expose Drupal's strengths and downplay its weaknesses where possible.
   An honest approach to the underlying framework and architecture
   of Drupal will be less confusing to the user down the road.

Admin is not an original work - many of its decisions have had direct
influences from other work in the community:

- [Administration menu](http://drupal.org/project/admin_menu)
  Daniel Kudwien (sun)

- [Navigate](http://drupal.org/project/navigate)
  Chris Shattuck (chrisshattuck)

- [d7ux](http://www.d7ux.org)
  Mark Boulton & Leisa Reichelt


Installation
============
1. Install & enable the module.

2. Admin makes a permission available that allows only properly
   permissioned users to make use of the admin toolbar. Users with the
   'use admin toolbar' permission will be able to use the toolbar.

3. You can configure the layout, position, and enabled tools for the
   admin toolbar on `admin/settings/admin`.


Implementing your own Admin "plugins"
=====================================
Admin's "plugins" are simply Drupal blocks. In order to expose one of your
module's blocks as one suitable for use in the admin toolbar you can set the `admin` key in your `hook_block()` to `TRUE`. Note that users can add any blocks to the admin toolbar if they wish at `admin/settings/admin`, though not all will work well in the context of the admin toolbar.

    /**
     * Implementation of hook_block().
     */
    function my_module_block($op = 'list', $delta = 0, $edit = array()) {
      switch ($op) {
        case 'list':
          $blocks = array();
          $blocks['example'] = array(
            'info' => t('Example block'),
            'admin' => TRUE
          );
          return $blocks;
      }
    }


Theming your block and other tips
=================================
Your block should provide general rules for either of the admin toolbar
layouts (horizontal or vertical). You can specify CSS rules using the
following selectors:

    #admin-toolbar.horizontal {}
    #admin-toolbar.vertical {}

Admin provides fairly decent defaults for the following Drupal core
theme functions:

  - `theme('item_list')`
  - menu output (e.g. `menu_tree_output()`).
  - most form elements (with the exception of fieldsets)
  - admin panes (see below)

Admin provides an additional FormAPI element type `admin_panes`. Admin
panes allow you to fit multiple elements of content into a togglable
interface. The panes will automatically adjust to the layout to the toolbar
and display as either vertical tabs (horizontal layout) or accordian boxes
(vertical layout).

Here is an example of using admin panes in a form API array:

    $form['panes'] = array(
      '#tree' => FALSE,
      '#type' => 'admin_panes',
      'foo' => array(
        '#title' => t('Pane 1'),
        ...
      ),
      'bar' => array(
        '#title' => t('Pane 2'),
        ...
      ),
    );

Note that each child element must have a #title attribute to be labeled
properly.


Contributors
============
- yhahn (Young Hahn)
- ajashton (AJ Ashton)
