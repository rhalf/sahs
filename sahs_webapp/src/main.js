import Vue from 'vue'
import App from './App.vue'
import router from './router'
import store from './store'
import vuetify from './plugins/vuetify';
import filter from './plugins/filter';

import VueTelInput from 'vue-tel-input'

import 'material-design-icons-iconfont/dist/material-design-icons.css'

Vue.config.productionTip = false

Vue.use(VueTelInput)


var app = new Vue({
  filter,
  store,
  router,
  vuetify,
  render: h => h(App)
})

app.$mount('#app')