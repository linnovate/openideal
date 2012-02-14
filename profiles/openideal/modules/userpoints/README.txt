
Copyright 2005-2008 http://2bits.com

Description
-----------
The userpoints and userpoints_nc module provides the ability for users to gain
points with the do certain actions, such as:

- posting a node (different points can be awarded for different
  node types, e.g. page, story, forum, image, ...etc.)
- posting a comment
- moderating a comment

Upon deleting a node or a comment the number of points is subtracted.
If a node or comment author is changed points are transferred respectively

The number of points for each of the above actions is configurable by
the site adminsitrator.

A transaction log is created for each event. The log is viewable by
the admin.

Points can be moderated, i.e. approval can be done by the admin at a later
time.

A block displays the number of points the user gained. Another block 
displays the top 5 users who earned points.

----
Using modules from the project http://drupal.org/project/userpoints_contrib 
point can be awarded for other actions. 
including:
- voting on a node (requires the nodevote module)
- referring a person to the site (requires referral module)
- a visitor comes to the site via clicking on an affiliate link
  (requires the affiliates module)
- voting up or down a node (requires the vote_up_down module)
- inviting a person to register on the site (requires invite module)
- invited person actually registers on the site
- purchasing from your e-commerce store (reward points)

Using real money, users can purchase points from your ecommerce store
as well. Moreover, the points can be used as currency for ecommerce as well,
as in a form of payment


This module is useful in providing an incentive for users to participate
in the site, and be more active. The module is easily extended through use of 
the API (see below)


Initially sponsored by: http://artalyst.com

Installation
------------
To install this module, do the following:

1. Extract the tar ball that you downloaded from Drupal.org.

2. Upload the userpoints directory and all its contents to your
   modules directory.

Configuration
-------------
To enable this module do the following:

1. Go to Admin -> Modules, and enable userpoints.
   Check the messages to make sure that you did not get any errors
   on database creation.

2. Go to Admin -> Settings -> userpoints.

   Configure the options as per your requirements

3. Go to Admin -> Access Control and enable viewing for the roles you want.

For configuring with e-commerce, you have to have the ecommerce modules
installed and configured.

- User points can be used as a form of payment, with an admin defined
  multiplier

- Users gain points when purchasing items via e-commerce for every dollar
  they spend.

This is useful as a reward system.

This also allows purchasing of points for real money. You have to setup
a non-shippable product, and adjust the multiplier accordingly.

API
---
This modules provides an application programming interface (API), which is
callable and actionable by other modules.

The functions are:

userpoints_userpointsapi()

  Accepts an integer or an array. 
  If the parameter is an integer it is assumed to be points 
  for the currently logged in user (i.e. global $user; $user->uid) 

  If the parameter is an array the array can contain one or more of the
  following options. The only required parameters are 'points' or 'txn_id'
  If a parameter is not set the site settings will used. Setting a parameter 
  to NULL will cause the entry to be NULL, defaults are only used if the 
  parameter is not set

  Returns an array with a status (true/false) and a reason (string) if there
  is an error. example
  return array('status' => false, 'reason' => 'DB transaction failed');
  
  'uid'         => (int) User ID 
  'points'      => (int) # of points to award the user 
  'txn_id'      => (int) Transaction ID of a current points record. If
                         present an UPDATE occurs
  'moderate'    => (boolean) TRUE or FALSE. If NULL site settings are adhered to
  'description' => (string) fulltext Description presented to the user
  'expirydate'  => (timestamp) timestamp the date/time when the points will
                               be expired (depends on cron)
  'event'       => (string) varchar32 descriptive identifier administrative purposes
  'reference'   => (string) varchar32 indexed/searchable field on the DB
  'display'     => (boolean) Whether or not to display the "Points awarded"
                             message. If null, fall back to USERPOINTS_DISPLAY_MESSAGE
  'tid'         => (int) Taxonomy ID to place these points into; MUST BE in
                         the userpoints Vocabulary!

  Examples
    //Add 5 points to the currently logged in user
    userpoints_userpointsapi(5);  

    //Also add 5 points to the currently logged in user
    $params = array (
      'uid' => $user->uid,
      'points' => 5,
    );
    userpoints_userpointsapi($params); 

  
//---Hooks
hook_userpoints($op, $params = array()) 

  Use this hook to act upon certain operations. When other modules award
  points to a user, your hook will be called, among others.

  The arguments are:

  $op: The operation to be acted upon.
    'setting'
      Pass a field set and fields that would be displayed in the userpoints
      settings page. For example, this can be used for your program to ask
      the admin to set a number of points for certain actions your module
      performs. The function should return an array object conforming to
      FormsAPI structure.

    'points before'
      Calls your module, and others, before the points are processed. You can
      prevent points from being awarded by returning FALSE.

    'points after'
      Calls your module, and others, after points are processed. You can take
      certain actions if you so wish. Return value is ignored.

   The $params variable is the original $params array as sent to userpoints_userpointsapi
 
//---Other useful functions

userpoints_get_current_points($uid = NULL, $tid = NULL);
  Returns an integer of the sum of the user's point 
  If a tid is passed in that category's sum is returned otherwise
  the sites default category is used

userpoints_get_max_points($uid = NULL, $tid = NULL);
  Returns an integer of the sum of the user's max points achieved
  If a tid is passed in that category's sum is returned otherwise
  the sites default category is used

userpoints_get_vid()
  Returns an integer of the userpoints Vocabulary

userpoints_get_default_tid()
  Returns an integer for the userpoints default Taxonomy ID
  Note: this is the default when submitting points so you 
        DO NOT need to pass this into userpoints_userpointsapi

userpoints_get_categories()
  Returns an array of the possible categories including
  the special "General" category (id=0). This is a keyed
  array that works perfectly with FAPI. If you're creating a 
  settings page wherein a user would select the category to 
  place points into, this will give you exactly what you need.
  See userpoints.module admin_settings function for an example.

userpoints_get_default_expiry_date()
  Returns a UNIX timestamp of the site's default expiration date.
  If an expiration date (or interval) it will be returned otherwise NULL

XML-RPC
-------

Using the userpoints_services module, and the services modules, you 
can allow external applications to query and update points on your
site.

Please refer to the services module documentation for further information.

Userpoints provides the following XML-RPC calls:

userpoints.get 

  string api_key (required)
    A valid API key.
  int uid (required)
    A valid Drupal User ID.
  int tid (optional)
    An optional Term ID for the category.

Example:

  $result = xmlrpc($server_url, 'userpoints.get', $key, $uid, $tid);
  // $result is an array
  // 'points' => 123

userpoints.points

  string api_key (required)
    A valid API key.
  int uid (required)
    A valid Drupal User ID.
  int points (required)
    Number of points to add/subtract.
  int tid (optional)
    An optional Term ID for the category.
  string event (optional)
    An optional event ID for this transaction.
  string description (optional)
    An optional description of this transaction.

Example:

  $result = xmlrpc($server_url, 'userpoints.points', $key, $uid, $points, $tid, $event, $description);
  // $result is an array
  // 'status'
  //   1 => Success
  //   0 => Fail
  // 'reason'
  //   Textual reason for failure, if status is 0

Bugs/Features/Patches:
----------------------
If you want to report bugs, feature requests, or submit a patch, please do so
at the project page on the Drupal web site.
http://drupal.org/project/userpoints

Author
------
Khalid Baheyeldin (http://baheyeldin.com/khalid and http://2bits.com)

If you use this module, find it useful, and want to send the author
a thank you note, then use the Feedback/Contact page at the URL above.

The author can also be contacted for paid customizations of this
and other modules.

