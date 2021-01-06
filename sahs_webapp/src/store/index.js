import Vue from 'vue';
import Vuex from 'vuex';

Vue.use(Vuex)

export default new Vuex.Store({
    state: {
        notify: {
            message: "Welcome to sahs",
            status: 'success',
            visible: true
        },
        user : {}
    },
    mutations: {
        notifyOpen: (state, payload) => {
            state.notify.message = payload.message
            state.notify.status = payload.status
            state.notify.visible = payload.visible
        },
        notifyClose: (state) => {
            state.notify.visible = false
        },
        setUser:(state, payload) => {
            state.user = payload
        }
    },
    actions: {
        notifyOpen: (contex, payload) => {
            contex.commit('notifyOpen', payload);
        },
        notifyClose: (contex) => {
            contex.commit('notifyClose');
        },
        setUser: (contex) => {
            contex.commit('setUser');
        }
    },
    getters: {
        notify: state => {
            return state.notify
        },
        user: state => {
            return state.user
        }
    }
});
