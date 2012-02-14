Description
-----------------
This module controls where users go after logging in:
 
- "return them to last page they were on" - a special checkbox
 (NOTE: in order for this to work on the /user page you have to follow the tutorial on the project page:
   http://drupal.org/project/login_destination . It works fine with the login block.)

  or

- a static URL you provide
  - internal drupal path ( "user", "example_page/subcategory" - no quotations . <front> supported in static / PHP mode.)
  - external URL - http://example.com, http://sub1.example.com, http://example.com/subdir  
  - aliases work, query strings too. For anchors - submit an issue.
  
  or
  
- your a PHP snippet that 
  - returns a string (all for static above applies here too) 
  - or an array: (ONLY when there is a GET query at the end of the url - like /example?var1=value)
   Example:
        return array('path' => 'node/add/video', 'query' => 'param1=100&param2=200');
        (note the absence of the "?" sign)
    
  - (see below for examples). The project page might have more: http://drupal.org/project/login_destination
  - PHP snippet's power is that you can have conditions in there (redirect based on uid, role, other logic)


Global go/no-go Redirect Filter:
---------------
    You can also configure from which pages redirection is applied only.
To achieve this you can specify either a list of URLs (wildcards do not work currently) 
or a PHP snippet to build this list dynamically. There is an "Always" checkbox that makes this filter fire redirection always.
So, you can redirect the user to various pages depending on which pages they are logging from.


Configuration
-----------------
Go to Administer -> User management -> Login destination page ( admin/user/login_destination ).



Example 1
-----------------
    Always redirect user to whatever page he was on when he logged in.
    
    Simply check "Always" in the condition fieldgroup and then
    check "Return user to where he/she came from. (Preserve destination)".
    
    Done.
    
( NOTE: This returns the user always to where he was - no matter if the "destination" $_GET variable was set explicitly in the URL.
If it was - it is used. If it was not set - the user is brought back. )

( NOTE 2: If you need this to work on the login page at /user  - you should read a howto on the http://drupal.org/project/login_destination )



Example 2
-----------------
PHP snippet for redirection URL should return a string. Here is an example:

  global $user;
  if ($user->uid == 1) {
    // Redirect the Administrator to "admin"
    return 'admin';
  } elseif ($user->uid == 2) {
    // Redirect the Site Owner to 'create video' page
    return array('path' => 'node/add/video', 'query' => 'param1=100&param2=200');
  } else {
    return 'node';
  }


Example 3
-----------------
PHP snippet for Redirection Condition should return boolean value. An example is:

  return ($_GET['q'] == 'user/login');
  
  or 
  
  return TRUE;
  
  or
  
  if ("A" == "B") {
    return FALSE;
  }
  else {
    return TRUE;
  }

Don't put PHP tags when creating snippets.

-------------------------


Authors
-----------------
Moshe Weitzman <weitzman AT tejasa DOT com>
ARDAS group <info AT ardas DOT dp DOT ua>
rsvelko from segments.at
