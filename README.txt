 - OpenideaL -
--------------------------------------------------------------------------------

Index

1. HybridAuth installation & configuration

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
