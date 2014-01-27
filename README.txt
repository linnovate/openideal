 - OpenideaL -
--------------------------------------------------------------------------------

Index

1. HybridAuth installation & configuration
2. Alerts & Notifications

- 1. HybridAuth installation & configuration
--------------------------------------------------------------------------------

1. Download the HybridAuth library:
http://sourceforge.net/projects/hybridauth/files/hybridauth-2.1.2.zip/download
2. Extract the archive to sites/all/libraries/hybridauth
(or profiles/idea/libraries/hybridauth)
3. Enable the HybridAuth module
4. Go to admin/config/people/hybridauth and enable all authentication providers
you whish to use
5. Click on the "Settings" link for every enabled provider and add the
authentication keys supplied by the provider
6. To automatically assign values from a service to user fields, create a rule
using the event "User registered through HybridAuth" and set the data values

- 2. Alerts & Notifications
--------------------------------------------------------------------------------

Notifications are e-mails that get sent by the system to alert users of the site
of certain events.
Drupal rules are used for its configuration and can be modified here:
admin/config/workflow/rules

There are notifications for the following events:

User notifications:
- When an idea is created in a challenge he is following
  Rule: Send mail for new idea
- When a comment is posted in a idea he is following
  Rule: Send mail for idea status change
- When he is mentioned in a comment or idea
  Rule: Send mail for new user
- When the status of one of his ideas has changed
  Rule: Send mail notification about the mention

Admin notifications:
- When a new user has registered

In the user profile, notifications for the following notification types can be
configured:

- Notifications for new content
- Notifications for new comments
- Notifications for mentions
