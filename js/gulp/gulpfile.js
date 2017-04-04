const gulp = require('gulp');
const sass = require('gulp-sass');
const autoprefixer = require('gulp-autoprefixer');

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