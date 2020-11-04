let gulp = require('gulp'),
sass = require('gulp-sass'),
rename = require('gulp-rename'),
postcss = require('gulp-postcss'),
autoprefixer = require('autoprefixer'),
rtlcss = require('gulp-rtlcss'),
browserSync = require('browser-sync').create();
require('dotenv').config({ path: process.cwd() + '/../../../../../../.env' });

const paths = {
  scss: {
    src: './scss/style.scss',
    dest: './css',
    watch: './scss/**/*.scss',
    bootstrap: './node_modules/bootstrap/scss/bootstrap.scss',
  },
  js: {
    bootstrap: './node_modules/bootstrap/dist/js/bootstrap.min.js',
    jquery: './node_modules/jquery/dist/jquery.min.js',
    popper: 'node_modules/popper.js/dist/umd/popper.min.js.map',
    dest: './js'
  }
}

// Compile sass into CSS & auto-inject into browsers
function styles() {
  return gulp.src([paths.scss.src])
    .pipe(sass().on('error', sass.logError))
    .pipe(postcss([autoprefixer({
      browsers: [
        'Chrome >= 35',
        'Firefox >= 38',
        'Edge >= 12',
        'Explorer >= 10',
        'iOS >= 8',
        'Safari >= 8',
        'Android 2.3',
        'Android >= 4',
        'Opera >= 12']
    })]))
    .pipe(gulp.dest(paths.scss.dest))
}

function stylesRtl() {
  return gulp.src([paths.scss.src])
    .pipe(sass().on('error', sass.logError))
    .pipe(rtlcss())
    .pipe(postcss([autoprefixer({
      browsers: [
        'Chrome >= 35',
        'Firefox >= 38',
        'Edge >= 12',
        'Explorer >= 10',
        'iOS >= 8',
        'Safari >= 8',
        'Android 2.3',
        'Android >= 4',
        'Opera >= 12']
    })]))
    .pipe(rename({ suffix: '-rtl' }))
    .pipe(gulp.dest(paths.scss.dest))
}

// Move the javascript files into our js folder
function js() {
  return gulp.src([paths.js.bootstrap, paths.js.jquery, paths.js.popper])
    .pipe(gulp.dest(paths.js.dest))
}

// Static Server + watching scss/html files
function serve() {
  browserSync.init({
    proxy: 'http://' + process.env.PROJECT_BASE_URL,
  })

  gulp.watch([paths.scss.watch], gulp.parallel(styles, stylesRtl)).on('change', browserSync.reload)
}

// Watch scss files.
function watch() {
  gulp.watch([paths.scss.watch], gulp.parallel(styles, stylesRtl))
}

const build = gulp.series(styles, gulp.parallel(js, watch))
const dev = gulp.series(styles, gulp.parallel(js, serve))

exports.styles = styles
exports.js = js
exports.serve = serve

exports.default = build
exports.dev = dev
