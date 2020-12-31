<template>
  <v-container fluid fill-height class="grey lighten-3">
    <v-row>
      <v-col></v-col>
      <v-col>
        <v-card fluid width="600">
          <v-app-bar class="blue lighten-1" flat dark>
            <v-icon>person</v-icon>
            Sign up
          </v-app-bar>
          <form class="px-16 py-6">
            <v-text-field
              type="email"
              v-model="email"
              label="E-mail"
              @blur="checkEmail()"
              required
            ></v-text-field>
            <v-text-field
              type="password"
              v-model="password1"
              label="Enter password"
              required
            ></v-text-field>
            <v-text-field
              type="password"
              v-model="password2"
              label="Retype password"
              required
            ></v-text-field>
          </form>

          <v-snackbar v-model="snackbar.show" :color="snackbar.state"
            >{{ snackbar.message }}
            <template v-slot:action="{ attrs }">
              <v-btn text v-bind="attrs" @click="snackbar.show = false"
                >Close
              </v-btn>
            </template>
          </v-snackbar>
          <!-- <v-alert class="mx-6" dense v-show="error.show" v-bind:type="error.state" >{{error.message}}</v-alert> -->

          <v-app-bar class="blue lighten-1 overflow hidden bottom">
            <v-btn text @click="signin()" dark>Signin</v-btn>
            <v-spacer></v-spacer>
            <v-btn text @click="submit()" dark>submit</v-btn>
          </v-app-bar>
        </v-card>
      </v-col>
      <v-col></v-col>
    </v-row>
    <v-row></v-row>
  </v-container>
</template>

<script>
import firebase from "../plugins/firebase";

export default {
  created() {
    if (firebase.auth().currentUser) this.$router.push("/dashboard");
  },
  updated() {},
  data() {
    return {
      email: "",
      password1: "",
      password2: "",
      snackbar: {
        message: "",
        state: "success",
        show: false,
      },
    };
  },
  methods: {
    submit: function () {
      if (!this.checkEmail()) return;
      if (!this.checkPassword()) return;
      firebase
        .auth()
        .createUserWithEmailAndPassword(this.email, this.password1)
        .then(
          () => {
            this.user = firebase.auth().currentUser;

            console.log(this.user);

            this.user.sendEmailVerification();

            this.snackbar.message = "Email verification has been sent!";
            this.snackbar.state = "success";
            this.snackbar.show = true;
          },
          (error) => {
            this.snackbar.message = error;
            this.snackbar.state = "error";
            this.snackbar.show = true;
          }
        );
    },
    signin: function () {
      this.$router.push("/");
    },
    checkPassword: function () {
      if (this.password1 != this.password2) {
        this.snackbar.message = "Password did not match!";
        this.snackbar.state = "warning";
        this.snackbar.show = true;
        return false;
      }
      if (this.password1.length == 0) {
        this.snackbar.message = "Password could not be empty!";
        this.snackbar.state = "warning";
        this.snackbar.show = true;
        return false;
      }
      if (this.password1.length < 6) {
        this.snackbar.message = "Password is less than 6 letters!";
        this.snackbar.state = "warning";
        this.snackbar.show = true;
        return false;
      }

      this.snackbar.show = false;
      return true;
    },
    checkEmail: function () {
      var re = /\S+@\S+\.\S+/;
      if (!re.test(this.email)) {
        this.snackbar.message = "Not a valid email!";
        this.snackbar.state = "warning";
        this.snackbar.show = true;
        return false;
      }

      this.snackbar.show = false;
      return true;
    },
  },
  computed: {},
};
</script>