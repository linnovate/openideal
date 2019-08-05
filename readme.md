# OpenideaL

## Index

- Installation
- HybridAuth installation & configuration
- Alerts & Notifications
- Activity

## Installation
Installing OpenideaL is easily done in one of the following methods:

- Using the full tarball from [drupal.org](https://www.drupal.org/project/idea) (**Important**: the latest development is done on GitHub, and so the version on drupal.org will always lag a bit behind)
- By running `drush make` command as follows: `drush make /path/to/build-openideal-github.make /path/to/your/destination/folder` .
- In both methods, once you're done preparing the installation files, visit install.php using your browser, to initiate OpenideaL's database.

## HybridAuth installation & configuration

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

## Alerts & Notifications

Notifications are e-mails that get sent by the system to alert users of the site
of certain events.
Drupal rules are used for its configuration and can be modified here:
admin/config/workflow/rules

There are notifications for the following events:

**User notifications:**

- When an idea is created in a challenge he is following
  Rule: Send mail for new idea
- When a comment is posted in a idea he is following
  Rule: Send mail for idea status change
- When he is mentioned in a comment or idea
  Rule: Send mail for new user
- When the status of one of his ideas has changed
  Rule: Send mail notification about the mention

**Admin notifications:**

- When a new user has registered

In the user profile, notifications for the following notification types can be
configured:

- Notifications for new content
- Notifications for new comments
- Notifications for mentions

## Activity

For the activity streams the message module is used. On the frontpage a number 
of different streams are show in a mini-panel called latest and greatest. Also 
an overal activity stream is available as a view (block).

**Main activity stream**

- New content (idea, challenge, news)
- New comments
- New users
- Various aggregated activity
 - received 10, 25, 50, 100 votes (x people have voted on [title])
 - reveived 10, 25, 50, 100 comments (x comments have been made for [title])
 - new users (x new users have joined this (week, month, year))

**Users**

- Posted x ideas
- User has joined

**Ideas**

- Idea was shared
- Idea won challenge
- Idea was posted

**Discussions**

- New comment
- x-th votes on comment

## Docker Containerization

This application is available in Docker container format from the Docker Hub via `danjng/openideal` (this may change as code is merged into the upstream repo of `linnovate/openideal`). It can be pulled by performing a `docker pull danjng/openideal`. To set up a demo environment, a `docker-compose.yml` file is provided. The file will run as-is, but can be customized appropriately to suit needs. The demo environment can be stood up by performing a `docker-compose -f "docker-compose.yml" up -d --build`.

This is a work in progress and may be subject to change. 

### Prerequisites

In order for this container to work, you will need the following fully installed and configured:

* Docker Desktop
  * Docker
  * Docker Compose
