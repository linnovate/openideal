The Login Destination module provides a way to customize the destination that a
user is redirected to after logging in, registering to the site, using a
one-time login link or logging out. 

The configuration consists of specifying so called login destination rules that
are evaluated when the login or logout takes place. Those rules are evaluated
against certain conditions and the user is taken to the destination specified
by the first matching rule. If the destination is empty, no redirect is
performed aka user is taken to the default destination. You can define pages
from which a user logs in/out to be a matching criterion. You can also select
certain user roles that are matched against those of a user. Note that only one
role has to match in order for the redirect to take place. If no roles are
selected the redirect is performed regardless of user roles. You can also
provide your own conditions by specifying PHP snippets (the PHP Filter has to
be enabled). The snippet should return TRUE if the condition matches and FALSE
otherwise.

There are no separate triggers for login and registration; instead you can
differentiate them by the specifying the pages that a user comes from:
- user - the user login form.
- user/register - the user registration form.
- user/*/edit - one-time login link, after the user has set the password.
- other - the login block or login forms embedded by other modules.
Please note that a user will be redirected when they register, even though they
may not be logged in afterwards immediately (e.g. because of email validation).
You can use this behavior to send the user to a page that contains further
instructions.

The destination you specify can be an internal page or an external URL.
Remember to precede the url with http://. You can also use the <front> tag to
redirect to the front page or the <current> tag to redirect to the page where
the user was before login/logout, aka the current page. In case of
login/register form the page from which the user entered the form is treaded as
the current page. Note that if you provide your own login/logout links you have
to add the 'current' GET parameter to them so Login Destination knows where
your users come from.

In some cases you will also need to provide the destination in a dynamic way by
using PHP snippets (the PHP Filter has to be enabled). The snippet's return
variable can be a string, for straight pages and urls, or an array for more
advanced options. The array should be in a form that the url function will
understand, e.g. %example. For more information, see the online API entry for
 <a href="@url">url function</a>. In most cases you will use it to specify GET
parameters and an anchor ("#"). Please study the examples below:

Take the user to the administration panel with underlying blog page:

<?php return array('blog', array('fragment' => 'overlay=admin/config', ), ); ?>

Take the user to the front page and specify some custom parameters:

<?php return array('<front>', array('query' => array('param1' => 'value1',
'param2' => 'value2', ), ), ); ?>

Take the user to the default page:

<?php return NULL; ?>

It also possible to set some advanced parameters on the setting page. Every
time in Drupal you can specify the 'destination' GET parameter in url to
redirect the user to a custom page. If you check the option
'Preserve the destination parameter' Login Destination will give priority to
this parameter over its own module settings. However with this option enabled
the redirect from the login block will not work. In some rare cases you can
also redirect the user just after using the one-time login link, before given
the possibility to change their password. Do this by checking the
'Redirect immediately after using one-time login link' option.