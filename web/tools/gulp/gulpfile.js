const gulp = require('gulp');
const sass = require('gulp-sass');
const autoprefixer = require('gulp-autoprefixer');
const babel = require('gulp-babel');
const uglify = require('gulp-uglify');
const sourcemaps = require('gulp-sourcemaps');
const browserSync = require('browser-sync').create();

gulp.task('sass', () =>
    gulp.src('src/app.scss')
        .pipe(sass())
        .pipe(autoprefixer({
            browsers: ['> 0%'],
            cascade: false
        }))
        .pipe(gulp.dest('dist'))
);

gulp.task('sass:watch', () =>
    gulp.watch('src/app.scss', ['sass'])
);

gulp.task('js', () =>
    gulp.src('src/app.js')
        .pipe(sourcemaps.init())
        .pipe(babel({
            presets: ['es2015']
        }))
        .pipe(uglify())
        .pipe(sourcemaps.write())
        .pipe(gulp.dest('dist'))
);

gulp.task('js:watch', () =>
    gulp.watch('src/app.js', ['js'])
);

gulp.task('html', () =>
    gulp.src('src/index.html')
        .pipe(gulp.dest('dist'))
);

gulp.task('html:watch', () =>
    gulp.watch('src/*.html', ['html'])
);

gulp.task('browser-sync', () => {
    browserSync.init({
        server: {
            baseDir: './dist'
        }
    });
    gulp.watch('dist/*').on('change', browserSync.reload);
});

gulp.task('default', [ 'sass', 'sass:watch', 'js', 'js:watch', 'html', 'html:watch', 'browser-sync']);