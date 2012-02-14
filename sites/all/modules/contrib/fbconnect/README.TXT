/* $Id: README.TXT,v 1.18.2.7 2010/10/30 12:18:35 vectoroc Exp $ */


-- SUMMARY --

This module roughly acts like the OpenId Drupal 6.x core module :
- It lets you use your Facebook ID and password to sign-in to your drupal site.
- Users can see which of their Facebook friends had an account on your drupal site.
- Users can publish a feed on their Facebook feed announcing that they have created an account or commented an article on your website.
- Users can invite their Facebook friends to create an account on the Drupal website.
- In case the user already holds an account on the website: The user can choose to link his drupal account to his Facebook identity.

-- FACEBOOK CONNECT TAB --

This module adds a new Facebook Connect tab in the user's profile page. This tab allows to change: 
- the "Use my Facebook picture as user picture" setting.
This field can be seen only if user_pictures is enabled.
The profile picture will be displayed directly from Facebook via FBML.  

- the "Let my Facebook friends see me" setting
If this parameter is enabled, the user's will be visible by her Facebook friends.

Also this tab allows to unlink your Facebook account.

-- REQUIREMENTS --
PHP 5.2 or higher versions. 
Drupal 6.x. 
Facebook PHP Library: http://github.com/facebook/php-sdk/
Facebook API key: http://www.facebook.com/developers/


-- INSTALLATION --
  1. Upload the 'fbconnect' folder into your module directory and activate the module,
  
  2. Upload facebook-php-sdk (http://github.com/facebook/php-sdk/) into fbconnect module folder (or into libraries folder, 
    if you want to use it with the libraries module)
    
     To ensure that you installed correctly check file : <fbconnect folder>/facebook-php-sdk/src/facebook.php must be accessible
     
 -- OR -- 
  Use drush_make script instead of steps 1 and 2:
 
projects[] = fbconnect
libraries[facebook-php-sdk][download][type] = "get"
libraries[facebook-php-sdk][download][url] = "http://github.com/facebook/php-sdk/tarball/v2.1.1"
libraries[facebook-php-sdk][destination] = "modules/fbconnect"   
  
  3. Create a new facebook application: http://www.facebook.com/developers/createapp.php
    
  4. Enter the Site URL on Web Site settings tab (e.g. copy value from admin/settings/fbconnect field “Connect url”).
  
  5. Configure the module through admin interface (admin/settings/fbconnect) using the information provided by Facebook 
    (Application ID, Application Secret).  
     
  6. In order for the user's browser to correctly recognize XFBML tags, you need to specify that the page is in XHTML. 
    Edit the "page.tpl.php" file of your current theme. edit the <html> tag and add the facebook xmlns for rendering XFBML.
    
    Example : <html xmlns:fb="http://www.facebook.com/2008/fbml" 
                    xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" 
                    lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
       
    Note: The xmlns:fb attribute (xmlns:fb="http://www.facebook.com/2008/fbml") is required for proper rendering on IE. 
     (Namespaces don't have to point to a valid page, they just need to be a valid Internationalized Resource Identifier (IRI).
  
  7. Also make sure that your page.tpl.php contains <?php print $closure ?>     
     
-- FAQ --

Q: How to skip linking local account to FB account?
A: Enable "Disable linking accounts during registration" checkbox on admin/settings/fbconnect/apperance page 

Q: Why the connect button does not appear?
Q: Facebook Connect dialog says: "Invalid Argument: Given URL is not allowed by the Application configuration".
A: Check 5th step of installation instructions. Check for error messages on page admin/reports/status.

Q: Fbconnect redirects to the Facebook homepage in popup
Q: I'm getting error "Application Unavailable - The application you are trying to access is unavailable or restricted"
A: Try to disable all other facebook* modules. If you're using fb_social you have to enable fbconnect_fb_social.

Q: Cannot login to drupal site after sucessully logged in to facebook.
A: Check your Application Secret on admin/settings/fbconnect page.

Q: Fbconnect Causes W3C XHTML Validation To Fail
A: http://drupal.org/node/365584#comment-3539890

Still did not solve problem ? Check admin/reports/status, php logs, post issue on http://drupal.org/project/issues/fbconnect