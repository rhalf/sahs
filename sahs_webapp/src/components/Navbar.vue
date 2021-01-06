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

      <v-btn @click="logout()" text>
        <v-icon>exit_to_app</v-icon>
        Logout
      </v-btn>
    </v-app-bar>

    <v-navigation-drawer
      v-model="drawer"
      app
      dark
      class="blue lighten-1"
      width="300"
    >
      <template v-slot:prepend>
        <v-list-item three-line>
          <v-list-item-avatar>
            <!-- <img v-if="user" v-bind:src="user.photoURL" onerror="./assets/user.png"> -->
            <img src="../assets/user.jpg" />
          </v-list-item-avatar>
          <v-list-item-content>
            <v-list-item-title>{{ user.displayName }}</v-list-item-title>
            <v-list-item-title>{{ user.email }}</v-list-item-title>
            <v-list-item-title>{{ user.privilege }}</v-list-item-title>
          </v-list-item-content>
        </v-list-item>
      </template>
      <v-divider></v-divider>
      <v-list>
        <v-list-item-group>
          <v-list-item
            v-for="link in filterNavs"
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
import notify from "@/mixins/notify";
import firebase from "../plugins/firebase";
var fire = firebase.firestore();
var auth = firebase.auth();

export default {
  mixins: [notify],
  created() {
    auth.onAuthStateChanged((user) => {
      if (user) {
        var docRef = fire.collection("users").doc(user.uid);

        docRef
          .get()
          .then((doc) => {
            this.$store.dispatch("setUser", doc.data());
            this.user = doc.data();
            // console.log(this.user);
          })
          .catch(function (error) {
            this.notifyOpen(error, "error");
          });
      } else {
        this.user = null;
      }
    });
  },

  data() {
    return {
      user: {},
      drawer: false,
      links: [
        {
          icon: "dashboard",
          text: "Dashboard",
          route: "/dashboard",
          admin: false,
        },
        { icon: "notes", text: "Posts", route: "/posts", admin: false },
        { icon: "image", text: "Gallery", route: "/gallery", admin: false },
        { icon: "mdi-database", text: "Datas", route: "/datas", admin: false },
        { icon: "settings", text: "Controls", route: "/controls", admin: false },
        {
          icon: "mdi-information",
          text: "About",
          route: "/about",
          admin: false,
        },
        { icon: "person", text: "Users", route: "/users", admin: true },
      ],
    };
  },
  computed: {
    filterNavs() {
      if (this.user.privilege == "admin") {
        return this.links;
      } else {
        return this.links.filter(function (link) {
          if (link.admin == false) return link;
        });
      }
    },
  },
  methods: {
    logout: function () {
      firebase
        .auth()
        .signOut()
        .then(
          () => {},
          (error) => {
            this.notifyOpen(error, "error");
          }
        );
    },
  },
};
</script>