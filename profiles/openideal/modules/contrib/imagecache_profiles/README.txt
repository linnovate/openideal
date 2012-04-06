Imagecache_Profiles module allows you to set user profile pictures that are consistent throughout your site and allows avatars on the user profile, node pages and comments to be a different size. 

- Enable core's Image module
  -visit admin/config/media/image-styles page
  - create a new Image Styles with the following settings
    - preset namespace: user_image_default
      - select "Scale and Crop" from the Effect table
        - set width and height to 100
    - preset namespace: user_image_large
      - select "Scale and Crop" from the Effect table
        - set width and height to 200

- Download and enable the ImageCache_Profiles module as usual
  - Enable user pictures at admin/config/people/accounts
    - if setting a default picture it should use a relative url path (ex. public://default_picture.png)
    - set picture maximum dimensions to 1600x1400
    - set picture maximum file size to 1024
    - set your picture guidelines text to: "Photo must be larger than 200x200 pixels." To prevent upscaling, these dimensions should be the dimensions of your largest preset.
    - select the Image style to set the user picture size on a user's profile page
    - select the Image style to set the user picture size within comments
    - select the Image style to set the user picture size within nodes
    - select the Image style to set the default user picture size throughout the site
    - set picture minimum width in pixels: 200
    - set picture minimum height in pixels: 200
      - To prevent upscaling, these dimensions should be the dimensions of your largest preset.
    - save configuration

- Developer docs
  - Module implements hook_preprocess_user_picture()
  - Examples of usage

  $account->picture = $file_id; // Or assign a image-file object itself.
  // $account->picture->uri is ONLY used for theme_image_style().
  // Optionally assign a style to file.
  // $account->picture->style_name = 'medium';
  // Or account itself.
  // $account->user_picture_style = 'large';
  $output = theme('user_picture', array('account' => $account, 'user_picture_style' => 'thumbnail'));
