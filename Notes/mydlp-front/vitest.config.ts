import { fileURLToPath } from 'node:url'
import { mergeConfig, defineConfig, configDefaults } from 'vitest/config'
import viteConfig from './vite.config'

export default mergeConfig(
  viteConfig,
  defineConfig({
    test: {
      environment: 'jsdom',
      exclude: [...configDefaults.exclude, 'e2e/**'],
      root: fileURLToPath(new URL('./', import.meta.url)),
      coverage: {
        provider: 'v8',
        reporter: ['text', 'html', 'lcov'],
        include: ['src/**/*.{ts,js,vue}'],
        exclude: [
            '**/*.test.ts', 
            '**/*.spec.ts', 
            'e2e/**', 
            "src/App.vue", 
            "src/main.ts", 
            "src/api/websocket.ts", 
            "src/api/api.ts", 
            "src/components/AppContainer.vue", 
            "src/router/index.ts",
            "src/models/*"
          ],
      },
    },
  }),
)