module.exports = {
  env: {
    browser: true,
    es6: true,
  },
  extends: [
    'plugin:react/recommended',
    'airbnb',
  ],
  globals: {
    Atomics: 'readonly',
    SharedArrayBuffer: 'readonly',
  },
  parser: '@typescript-eslint/parser',
  parserOptions: {
    ecmaFeatures: {
      jsx: true,
    },
    ecmaVersion: 2018,
    sourceType: 'module',
  },
  plugins: [
    'react',
    '@typescript-eslint',
  ],
  rules: {
    'react/jsx-filename-extension': [
      2,
      {
        extensions: ['.tsx']
      }
    ],
    'import/extensions': [
      2,
      'never',
    ],
    'semi': [
      2,
      'never'
    ],
  },
  settings: {
    react: {
      version: 'detect',
    }
  }
};
