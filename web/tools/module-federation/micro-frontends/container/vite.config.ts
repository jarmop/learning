import { federation } from '@module-federation/vite'
import react from '@vitejs/plugin-react'
import { defineConfig } from 'vite'
import { dependencies } from './package.json'

export default defineConfig(() => {
  return {
    server: {
      fs: {
        allow: ['.', '../shared'],
      },
    },
    // preview: {
    //   allowedHosts: true,
    // },
    build: {
      target: 'chrome89',
    },
    plugins: [
      federation({
        name: 'container',
        remotes: {
          content1: {
            type: 'module',
            name: 'content1',
            entry: 'http://localhost:4174/remoteEntry.js',
            // entry: 'http://www.jarmo.com:4174/remoteEntry.js',
            entryGlobalName: 'content1',
            shareScope: 'default',
          },
        },
        exposes: {},
        filename: 'remoteEntry.js',
        shared: {
          react: {
            requiredVersion: dependencies.react,
            singleton: true,
          },
        },
      }),
      react(),
    ],
  }
})
