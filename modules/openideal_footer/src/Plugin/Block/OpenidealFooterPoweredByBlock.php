<?php

namespace Drupal\openideal_footer\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Extension\ModuleHandlerInterface;
use Drupal\Core\Extension\ThemeHandler;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\HttpFoundation\RequestStack;

/**
 * Provides a "Powered by" Block.
 *
 * @Block(
 *   id = "openidel_footer_powered_by",
 *   admin_label = @Translation("Powered by"),
 *   category = @Translation("Openideal")
 * )
 */
class OpenidealFooterPoweredByBlock extends BlockBase implements ContainerFactoryPluginInterface {

  /**
   * Theme manger.
   *
   * @var \Drupal\Core\Theme\ThemeManager
   */
  protected $themeHandler;

  /**
   * Config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory
   */
  protected $configFactory;

  /**
   * Request.
   *
   * @var \Symfony\Component\HttpFoundation\Request
   */
  protected $request;

  /**
   * Module handler.
   *
   * @var \Drupal\Core\Extension\ModuleHandlerInterface
   */
  protected $moduleHandler;

  /**
   * {@inheritDoc}
   */
  public function __construct(array $configuration,
                              $plugin_id,
                              $plugin_definition,
                              ThemeHandler $theme_handler,
                              ConfigFactory $config_factory,
                              RequestStack $requestStack,
                              ModuleHandlerInterface $moduleHandler
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->themeHandler = $theme_handler;
    $this->configFactory = $config_factory;
    $this->request = $requestStack->getCurrentRequest();
    $this->moduleHandler = $moduleHandler;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static($configuration, $plugin_id, $plugin_definition,
      $container->get('theme_handler'),
      $container->get('config.factory'),
      $container->get('request_stack'),
      $container->get('module_handler')
    );
  }

  /**
   * {@inheritDoc}
   */
  public function build() {
    $config = $this->configFactory->get('openideal_footer.openideal_footer_links_config');
    $path = $this->themeHandler->getTheme('openideal_theme')->getPath();
    $base_theme_path = base_path() . $path;
    $path = $this->moduleHandler->getModule('openideal_footer')->getPath();
    return [
      '#theme' => 'openideal_powered_by',
      '#site_url' => $config->get('openideal_official_site'),
      '#logo' => base_path() . $path . '/images/logo.webp',
      '#links' => [
        'github' => [
          'path' => $config->get('github'),
          'logo' => $base_theme_path . '/images/icons/github_logo.png',
          'alt' => $this->t('GitHub'),
        ],
        'twitter' => [
          'path' => $config->get('twitter'),
          'logo' => $base_theme_path . '/images/icons/twitter_logo.png',
          'alt' => $this->t('Twitter'),
        ],
      ],
      '#cache' => [
        'tags' => $config->getCacheTags(),
      ],
    ];
  }

}
