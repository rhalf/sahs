import Vue from 'vue'
import App from './App.vue'
import router from './router'
import vuetify from './plugins/vuetify';
import filter from './plugins/filter';

import 'material-design-icons-iconfont/dist/material-design-icons.css'

Vue.config.productionTip = false


new Vue({
  filter,
  router,
  vuetify,
  render: h => h(App)
}).$mount('#app')
