import { createRouter, createWebHistory } from 'vue-router'

import ListView from '@/views/ListView.vue'
import WhitelistView from '@/views/WhitelistView.vue'

const router = createRouter({
  history: createWebHistory(import.meta.env.BASE_URL),

  routes: [
    {
      path: '/',
      redirect: '/list/',
    },

    {
      path: '/list/',
      name: 'list',
      component: ListView,
    },

    {
      path: '/whitelist/',
      name: 'whitelist',
      component: WhitelistView,
    },
  ],
})

export default router