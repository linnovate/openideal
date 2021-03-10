<?php

namespace Drupal\openideal_user\EventSubscriber;

use Drupal\social_api\AuthManager\OAuth2ManagerInterface;
use Drupal\social_auth_facebook\FacebookAuthManager;
use Drupal\social_auth_github\GitHubAuthManager;
use Drupal\social_auth_linkedin\LinkedInAuthManager;
use Drupal\Core\Messenger\MessengerTrait;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\social_auth\Event\SocialAuthEvents;
use Drupal\social_auth\Event\UserEvent;
use Drupal\social_auth\Event\UserFieldsEvent;
use Drupal\social_auth_google\GoogleAuthManager;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

/**
 * Event subscriber to react on social auth events.
 */
class OpenidealSocialAuthSubscriber implements EventSubscriberInterface {

  use StringTranslationTrait;
  use MessengerTrait;

  const GOOGLE_PEOPLE_API_DOMAIN = 'https://people.googleapis.com';

  /**
   * Social plugins.
   */
  const GOOGLE_PLUGIN_ID = 'social_auth_google';
  const LINKEDIN_PLUGIN_ID = 'social_auth_linkedin';
  const FB_PLUGIN_ID = 'social_auth_facebook';
  const GITHUB_PLUGIN_ID = 'social_auth_github';

  /**
   * Google auth manager.
   *
   * @var \Drupal\social_auth_google\GoogleAuthManager
   */
  protected $googleAuthManager;

  /**
   * LinkedIn auth manager.
   *
   * @var \Drupal\social_auth_linkedin\LinkedInAuthManager
   */
  protected $linkedInAuthManager;

  /**
   * FB auth manager.
   *
   * @var \Drupal\social_auth_google\GoogleAuthManager
   */
  protected $fbAuthManager;

  /**
   * GitHub auth manager.
   *
   * @var \Drupal\social_auth_github\GitHubAuthManager
   */
  protected $gitHubAuthManager;

  /**
   * Mapped plugin id's with related methods.
   *
   * @var array
   */
  protected $socialsPlugins = [
    self::GOOGLE_PLUGIN_ID => 'googleAuthManager',
    self::LINKEDIN_PLUGIN_ID => 'linkedInAuthManager',
    self::FB_PLUGIN_ID => 'fbAuthManager',
    self::GITHUB_PLUGIN_ID => 'gitHubAuthManager',
  ];

  /**
   * OpenidealSocialAuthSubscriber constructor.
   *
   * @param \Drupal\social_auth_google\GoogleAuthManager $social_auth_google
   *   Google auth manager.
   * @param \Drupal\social_auth_linkedin\LinkedInAuthManager $social_auth_linkedin
   *   LinkedIn auth manager.
   * @param \Drupal\social_auth_facebook\FacebookAuthManager $social_auth_facebook
   *   FB auth manager.
   * @param \Drupal\social_auth_github\GitHubAuthManager $gitHubAuthManager
   *   GitHub auth manager.
   */
  public function __construct(GoogleAuthManager $social_auth_google, LinkedInAuthManager $social_auth_linkedin, FacebookAuthManager $social_auth_facebook, GitHubAuthManager $gitHubAuthManager) {
    $this->googleAuthManager = $social_auth_google;
    $this->linkedInAuthManager = $social_auth_linkedin;
    $this->fbAuthManager = $social_auth_facebook;
    $this->gitHubAuthManager = $gitHubAuthManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    return [
      SocialAuthEvents::USER_CREATED => 'socialAuthUserLogin',
      SocialAuthEvents::USER_FIELDS => 'socialAuthUserFields',
    ];
  }

  /**
   * This method is called when the USER_CREATED event is dispatched.
   *
   * @param \Drupal\social_auth\Event\UserEvent $event
   *   The dispatched event.
   */
  public function socialAuthUserLogin(UserEvent $event) {
    $this->messenger()->addMessage($this->t('Please fill your profile.'));
  }

  /**
   * This method is called when the USER_FIELDS event is dispatched.
   *
   * @param \Drupal\social_auth\Event\UserFieldsEvent $event
   *   The dispatched event.
   */
  public function socialAuthUserFields(UserFieldsEvent $event) {
    $plugin_id = $event->getPluginId();
    $user_fields = $event->getUserFields();
    if ($event->getSocialAuthUser()->getPicture() !== NULL) {
      $user_fields += [
        'avatars_user_picture' => $event->getSocialAuthUser()->getPicture(),
      ];
    }
    // Get data from socials and set it in user fields.
    if (array_key_exists($plugin_id, $this->socialsPlugins)) {
      /** @var \Drupal\social_api\AuthManager\OAuth2ManagerInterface $social_manager */
      $social_manager = $this->{$this->socialsPlugins[$plugin_id]};
      $this->setUserGender($social_manager, $plugin_id, $user_fields);
      $this->setEmail($social_manager, $plugin_id, $user_fields);
      $resource_owner = $social_manager->getUserInfo();

      if ($plugin_id !== self::GITHUB_PLUGIN_ID) {
        $user_fields += [
          'field_first_name' => $resource_owner->getFirstName() ?? '',
          'field_last_name' => $resource_owner->getLastName() ?? '',
        ];
      }
    }
    $event->setUserFields($user_fields);
  }

  /**
   * Set the user gender.
   *
   * @param \Drupal\social_api\AuthManager\OAuth2ManagerInterface $social_manager
   *   Social manager.
   * @param string $plugin_id
   *   Plugin id.
   * @param array $user_fields
   *   User fields.
   */
  private function setUserGender(OAuth2ManagerInterface $social_manager, string $plugin_id, array &$user_fields) {
    $resource_owner = $social_manager->getUserInfo();

    if ($plugin_id == self::FB_PLUGIN_ID && ($resource_owner_gender = $resource_owner->getGender())) {
      $gender = ($resource_owner_gender === 'female' || $resource_owner_gender === 'male') ? $resource_owner_gender : 'other';
      $user_fields += ['field_gender' => $gender];
    }

    if ($plugin_id == self::GOOGLE_PLUGIN_ID
      // Don't need to handle exceptions during request,
      // the social manager does. Request cannot be done
      // with end point because domain not the same as default.
      && ($genders = $social_manager->requestEndPoint('GET', '/v1/people/me?personFields=genders', self::GOOGLE_PEOPLE_API_DOMAIN))
      && isset($genders['genders'][0]['value'])) {
      $gender = $genders['genders'][0]['value'];
      if ($gender === 'unspecified') {
        $user_fields += ['field_gender' => 'other'];
      }
      else {
        $user_fields += ['field_gender' => $gender];
      }
    }
  }

  /**
   * Set the user email.
   *
   * @param \Drupal\social_api\AuthManager\OAuth2ManagerInterface $social_manager
   *   Social manager.
   * @param string $plugin_id
   *   Plugin id.
   * @param array $user_fields
   *   User fields.
   */
  private function setEmail(OAuth2ManagerInterface $social_manager, string $plugin_id, array &$user_fields) {
    // To get the email that was set as private
    // need to make additional request.
    if (!isset($user_fields['mail']) && $plugin_id == self::GITHUB_PLUGIN_ID) {
      $response = $social_manager->requestEndPoint('GET', '/user/emails');
      foreach ($response as $item) {
        if (isset($item['primary']) && $item['primary'] === TRUE && isset($item['email'])) {
          $user_fields['mail'] = $item['email'];
        }
      }
    }
  }

}
