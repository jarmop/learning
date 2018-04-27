var CustomPlugin = require('./custom-plugin');

module.exports = {
  mode: 'production',
  entry: './app.js',
  output: {
    filename: 'bundle.js'
  },
  plugins: [
      new CustomPlugin({customOption: 'custom option value'})
  ],
};