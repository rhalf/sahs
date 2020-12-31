<template>
  <div v-if="user">
    <v-app-bar flat height="50" app dark class="blue lighten-1">
      <v-app-bar-nav-icon @click="drawer = !drawer"></v-app-bar-nav-icon>
      <!-- <v-icon>mdi-notebook</v-icon> -->
      <v-toolbar-title class="text-uppercase white--text">
        <span class="font-weight-light">Sahs</span>
        0.0.1
      </v-toolbar-title>

      <v-spacer></v-spacer>
      <!-- <v-btn icon>
            <v-icon>mdi-magnify</v-icon>
        </v-btn> -->
      <v-btn icon>
        <img
          src="https://lipis.github.io/flag-icon-css/flags/4x3/ph.svg"
          height="24px"
          width="24px"
        />
      </v-btn>

      <v-btn icon @click="logout()">
        <v-icon>exit_to_app</v-icon>
      </v-btn>
    </v-app-bar>

    <v-navigation-drawer v-model="drawer" app dark class="blue lighten-1" width="300">
      <template v-slot:prepend>
        <v-list-item two-line>
          <v-list-item-avatar>
            <!-- <img v-if="user" v-bind:src="user.photoURL" onerror="./assets/user.png"> -->
            <img src="../assets/user.jpg" />
          </v-list-item-avatar>
          <v-list-item-content>
            <v-list-item-title v-if="user">{{user.email}}</v-list-item-title>
            <v-list-item-title v-if="user"> logged in </v-list-item-title>
          </v-list-item-content>
        </v-list-item>
      </template>
      <v-divider></v-divider>
      <v-list>
        <v-list-item-group>
          <v-list-item
            v-for="link in links"
            :key="link.text"
            router
            :to="link.route"
          >
            <v-list-item-icon>
              <v-icon>{{ link.icon }}</v-icon>
            </v-list-item-icon>
            <v-list-item-content>
              <v-list-item-title>{{ link.text }}</v-list-item-title>
            </v-list-item-content>
          </v-list-item>
        </v-list-item-group>
      </v-list>
    </v-navigation-drawer>
  </div>
</template>
<script>
import firebase from "../plugins/firebase";

export default {
  created() {
    firebase.auth().onAuthStateChanged(
      (theUser) => {
        this.user = theUser;
      },
      (error) => {
        console.log(error);
      }
    );
  },
  data() {
    return {
      user: {},
      drawer: false,
      links: [
        { icon: "dashboard", text: "Dashboard", route: "/dashboard" },
        { icon: "notes", text: "Posts", route: "/posts" },
        { icon: "image", text: "Gallery", route: "/gallery" },
        { icon: "mdi-database", text: "Datas", route: "/datas" },
        { icon: "mdi-information", text: "About", route: "/about" },
      ],
    };
  },
  methods: {
    logout: function () {
      firebase
        .auth()
        .signOut()
        .then(
          () => {},
          (error) => {
            console.log(error);
          }
        );
    },
  },
};
</script>