const gulp = require('gulp');
const babel = require('gulp-babel');
const uglify = require('gulp-uglify');
const sourcemaps = require('gulp-sourcemaps');
const browserSync = require('browser-sync').create();
const concat = require('gulp-concat');

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

gulp.task('js:vendor', () => {
    let vendorSrc = [
        'node_modules/react/dist/react.js',
        'node_modules/react-dom/dist/react-dom.js'
    ];

    gulp.src(vendorSrc)
        .pipe(uglify())
        .pipe(concat('vendor.js'))
        .pipe(gulp.dest('dist'))
});

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

gulp.task('default', ['js', 'js:vendor', 'js:watch', 'html', 'html:watch', 'browser-sync']);