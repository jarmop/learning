const gulp = require('gulp');
const sass = require('gulp-sass');
const autoprefixer = require('gulp-autoprefixer');
const babel = require('gulp-babel');
const uglify = require('gulp-uglify');
const sourcemaps = require('gulp-sourcemaps');

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