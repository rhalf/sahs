import Vue from 'vue'
import VueRouter from 'vue-router'

import Dashboard from '../views/Dashboard.vue'
import Gallery from '../views/Gallery.vue'
import Posts from '../views/Posts.vue'
import Datas from '../views/Datas.vue'
import About from '../views/About.vue'
import Signin from '../views/Signin.vue'
import Signup from '../views/Signup.vue'
import Users from '../views/Users.vue'
import Controls from '../views/Controls.vue'
import firebase from '../plugins/firebase'


Vue.use(VueRouter)

const routes = [
  // {
  //   path: '/tradingView',
  //   name: 'TradingView',
  //   component: TradingView,
  //   meta: {
  //     authenticated: false,
  //   }
  // },
  {
    path: '/',
    name: 'Signin',
    component: Signin,
    meta: {
      authenticated: false,

    }
  },
  {
    path: '/signup',
    name: 'Signup',
    component: Signup,
    meta: {
      authenticated: false,

    }
  },
  {
    path: '/dashboard',
    name: 'Dashboard',
    component: Dashboard,
    meta: {
      authenticated: true,

    }
  },
  {
    path: '/gallery',
    name: 'Gallery',
    component: Gallery,
    meta: {
      authenticated: true,

    }
  },
  {
    path: '/Posts',
    name: 'Posts',
    component: Posts,
    meta: {
      authenticated: true,

    }
  },
  {
    path: '/datas',
    name: 'Datas',
    component: Datas,
    meta: {
      authenticated: true,

    }
  },
  {
    path: '/users',
    name: 'Users',
    component: Users,
    meta: {
      authenticated: true,
    }
  },
  {
    path: '/controls',
    name: 'Controls',
    component: Controls,
    meta: {
      authenticated: true,
    }
  },
  {
    path: '/about',
    name: 'About',
    component: About,
    meta: {
      authenticated: true,
    }
  }
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})


router.beforeEach((to, from, next) => {
  firebase.auth().onAuthStateChanged(() => {
    if (to.matched.some((record) => record.meta.authenticated)) {

      if (firebase.auth().currentUser) {
        if (firebase.auth().currentUser.emailVerified) {
          next()
        } else{
          firebase.auth().signOut();
          next('/')
        }
      } else {
        firebase.auth().signOut();
        next('/')
      }
    } else {
      next()
    }
  }, (error) => {
    console.log(error.message)
  })
})

export default router
