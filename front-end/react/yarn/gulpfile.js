const gulp = require('gulp');
const babel = require('gulp-babel');
const uglify = require('gulp-uglify');
const sourcemaps = require('gulp-sourcemaps');
const browserSync = require('browser-sync').create();

gulp.task('js', () =>
    gulp.src('src/app.js')
        .pipe(sourcemaps.init())
        .pipe(babel({
            "presets": ["es2015", "react"]
        }))
        .pipe(uglify())
        .pipe(sourcemaps.write())
        .pipe(gulp.dest('dist'))
);

gulp.task('js:watch', () =>
    gulp.watch('src/app.js', ['js'])
);

// gulp.task('html', () =>
//     gulp.src('src/index.html')
//         .pipe(gulp.dest('dist'))
// );
//
// gulp.task('html:watch', () =>
//     gulp.watch('src/*.html', ['html'])
// );

gulp.task('browser-sync', () => {
    browserSync.init({
        server: {
            baseDir: './'
        }
    });
    gulp.watch('dist/*').on('change', browserSync.reload);

    // browserSync.init({
    //     server: {
    //         baseDir: './dist'
    //     }
    // });
    // gulp.watch('dist/*').on('change', browserSync.reload);
});

// gulp.task('default', ['js', 'js:watch', 'html', 'html:watch', 'browser-sync']);
gulp.task('default', ['js', 'js:watch', 'browser-sync']);