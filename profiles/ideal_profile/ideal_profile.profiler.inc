

name = IdeaL
core = 6.x
theme = ideal_theme

;core
dependencies[] = help
dependencies[] = menu
dependencies[] = path
dependencies[] = search
dependencies[] = dblog
dependencies[] = taxonomy

; Contrib
dependencies[] = ctools

dependencies[] = admin_menu
dependencies[] = admin
dependencies[] = adminrole
dependencies[] = advanced_help
dependencies[] = boxes
dependencies[] = backup_migrate
dependencies[] = backup_migrate_files
dependencies[] = corresponding_node_references
dependencies[] = content
dependencies[] = contextphp
dependencies[] = context_layouts
dependencies[] = context_ui
dependencies[] = devel
dependencies[] = diff
dependencies[] = date_api
dependencies[] = email_confirm
dependencies[] = fieldgroup
dependencies[] = filefield
dependencies[] = features
dependencies[] = faq
dependencies[] = flag
dependencies[] = googleanalytics
dependencies[] = heartbeat
dependencies[] = htmlmail
dependencies[] = imageapi
dependencies[] = imageapi_gd
dependencies[] = imagecache
dependencies[] = imagecache_ui
dependencies[] = imagefield
dependencies[] = install_profile_api
dependencies[] = logintoboggan
dependencies[] = markdown
dependencies[] = nodequeue
dependencies[] = nodereference
dependencies[] = nodewords
dependencies[] = nodewords_basic
dependencies[] = nodewords_nodetype
dependencies[] = number
dependencies[] = optionwidgets
dependencies[] = page_title
dependencies[] = pathauto
dependencies[] = print
dependencies[] = print_mail
dependencies[] = print_pdf
dependencies[] = quicktabs
dependencies[] = rules
dependencies[] = rules_admin
dependencies[] = strongarm
dependencies[] = service_links
dependencies[] = text
dependencies[] = token
dependencies[] = userpoints
dependencies[] = userpoints_rules
dependencies[] = views
dependencies[] = views_bulk_operations
dependencies[] = views_ui
dependencies[] = votingapi
dependencies[] = voting_rules
dependencies[] = vud
dependencies[] = vud_node
dependencies[] = wysiwyg
dependencies[] = webform

;custom
dependencies[] = ideal
dependencies[] = ideal_challenge
dependencies[] = ideal_popular
dependencies[] = ideal_project
dependencies[] = ideal_comments
dependencies[] = ideal_default_content

;features
dependencies[] = fe_main
dependencies[] = fe_project
dependencies[] = fe_challenge
dependencies[] = fe_news
;dependencies[] = default_content



roles[admin] = administrator


; -----------------------------------------------------------------------------
;                                   Users
; -----------------------------------------------------------------------------
; Keep in mind when adding users like this that you are proceeding at your own
; risk. You are writing *in plain text* certain data that could compromise your 
; site. The password field you specify for any user should be an md5 hashed 
; password. I guess that gives you at least one measure of security.
;
; One other thing to note here is that there's this (handy) quirk in Drupal's 
; install API in that you can bypass the configuration form during installation
; (where you'd normally set up user 1 credentials and the site name and such).
; To do this you need to specify credentials for user 1 and also specify the
; site name and site email variables (@see Variables below). 
; -----------------------------------------------------------------------------

;users[superduper][uid]    = 1
;users[superduper][name]   = admin
;users[superduper][mail]   = admin@admin
;users[superduper][pass]   = 10aa208e7e9e257a3f4fe66d1acf84e8
;users[superduper][roles]  = administrator
;users[superduper][status] = 1

;users[sitemanager][uid]    = 2
;users[sitemanager][name]   = John Lennon
;users[sitemanager][mail]   = john@beatles
;users[sitemanager][pass]   = cb6f3b8fa1aa7248aee240f594448a39
;users[sitemanager][roles]  = administrator
;users[sitemanager][status] = 1

;input-formats
input-formats[markdown][name] = Markdown
input-formats[markdown][default] = 1
input-formats[markdown][roles] = anonymous user,authenticated user
input-formats[markdown][filters][markdown][0] = -10
input-formats[markdown][filters][filter][0] = -7
input-formats[markdown][filters][filter][1] = -8
input-formats[markdown][filters][filter][2] = -9
input-formats[markdown][filters][filter][3] = -6
input-formats[markdown][allowed-html] = "<a> <p> <em> <strong> <span> <h1> <h2> <h3> <h4> <h5> <blockquote> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd> <img> <table> <thead> <tbody> <tfoot> <th> <tr> <td>"

input-formats[wysiwyg][name] = WYSIWYG
input-formats[wysiwyg][roles] = Admin,Publisher
input-formats[wysiwyg][filters][filter][0] = -8
input-formats[wysiwyg][filters][filter][1] = -7
input-formats[wysiwyg][filters][filter][2] = -9
input-formats[wysiwyg][filters][filter][3] = -6
input-formats[wysiwyg][allowed-html] = "<a> <p> <em> <strong> <span> <h1> <h2> <h3> <h4> <h5> <blockquote> <cite> <code> <ul> <ol> <li> <dl> <dt> <dd> <img> <table> <thead> <tbody> <tfoot> <th> <tr> <td>"
input-formats[wysiwyg][wysiwyg][editor] = tinymce
input-formats[wysiwyg][wysiwyg][settings][default] = 1
input-formats[wysiwyg][wysiwyg][settings][user_choose] = 0
input-formats[wysiwyg][wysiwyg][settings][show_toggle] = 0
input-formats[wysiwyg][wysiwyg][settings][theme] = advanced
input-formats[wysiwyg][wysiwyg][settings][language] = en
input-formats[wysiwyg][wysiwyg][settings][buttons][default][bold] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][italic] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][underline] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][bullist] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][numlist] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][outdent] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][indent] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][link] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][unlink] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][default][image] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][paste][pastetext] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][font][formatselect] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][font][styleselect] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][table][tablecontrols] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][safari][safari] = 1
input-formats[wysiwyg][wysiwyg][settings][buttons][imce][imce] = 1
input-formats[wysiwyg][wysiwyg][settings][toolbar_loc] = top
input-formats[wysiwyg][wysiwyg][settings][toolbar_align] = left
input-formats[wysiwyg][wysiwyg][settings][path_loc] = bottom
input-formats[wysiwyg][wysiwyg][settings][resizing] = 1
input-formats[wysiwyg][wysiwyg][settings][verify_html] = 1
input-formats[wysiwyg][wysiwyg][settings][preformatted] = 0
input-formats[wysiwyg][wysiwyg][settings][convert_fonts_to_spans] = 1
input-formats[wysiwyg][wysiwyg][settings][remove_linebreaks] = 1
input-formats[wysiwyg][wysiwyg][settings][apply_source_formatting] = 0
input-formats[wysiwyg][wysiwyg][settings][paste_auto_cleanup_on_paste] = 0
input-formats[wysiwyg][wysiwyg][settings][block_formats] = p,h2,h3,h4,h5,blockquote
input-formats[wysiwyg][wysiwyg][settings][css_classes] = "Highlight=highlight
Float Left=left
Float Right=right"


variables[site_name] = IdeaL
variables[site_mail] = info@ideal.com

variables[site_footer] = IdeaL by Linnovate
;variables[site_frontpage] = home
variables[theme_settings][toggle_node_info_page] = 0

; You may also want to set the clean url variable if you don't need clean urls. 
; Since we bypassed the configuration screen, it is enabled by default, so to 
; disable it you'll want to uncomment this line:
;variables[clean_url] = 0

; -----------------------------------------------------------------------------
;                                 Nodes types
; -----------------------------------------------------------------------------
node-types[page][type] = page
node-types[page][name] = Page
node-types[page][has_body] = 1
node-types[page][body_label] = Body
node-types[page][module] = node
node-types[page][description] = A <em>page</em> is simply what it sounds like - it's a page on your website.
node-types[page][custom] = 1
node-types[page][modified] = 1
node-types[page][locked] = 0
node-types[page][help] = ''
node-types[page][min_word_count] = ''


; -----------------------------------------------------------------------------
;                                    Nodes
; -----------------------------------------------------------------------------
;nodes[front][type] = page
;nodes[front][title] = Isn't Profiler nifty?
;nodes[front][uid] = 2
;nodes[front][body] = <p>Donec vel ipsum elit. In rutrum, ante at fringilla pharetra, mi justo fermentum risus; eget commodo est mi lobortis augue. Proin fringilla malesuada semper. Ut velit sapien, condimentum vel faucibus ut, aliquam quis dui? Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vivamus vulputate fringilla enim, non convallis nibh lacinia at. Etiam fermentum, lectus ac sagittis volutpat, est augue tincidunt mi, et consectetur orci nisi sit amet sapien. Ut nisi nunc, molestie quis sollicitudin at, dignissim eget nisl? Sed vitae sem lectus. Curabitur viverra laoreet magna. Sed eget sapien tellus, eu tempor purus. Proin dignissim, lacus vitae venenatis porttitor, justo libero rhoncus felis, nec dictum nisl mi vitae nisi. Maecenas tincidunt eros at turpis sollicitudin nec viverra enim condimentum. Sed nec enim justo. In tristique risus dui; ac egestas mi. Proin at arcu sed enim eleifend ornare eu at arcu. Curabitur ullamcorper, ipsum eu imperdiet pharetra, mauris lorem hendrerit metus, in consectetur mauris est iaculis eros. Quisque euismod hendrerit metus ullamcorper tempus! Cras gravida ornare arcu vitae commodo. In hac habitasse platea turpis duis.</p>
