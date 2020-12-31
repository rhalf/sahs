<template>
  <v-container fluid fill-height class="">
    <v-row>
      <v-col></v-col>
      <v-col>
        <v-card fluid width="600">
          <v-app-bar class="blue lighten-1" flat dark>
            <!-- <v-icon>person</v-icon> -->
            <v-toolbar-title class="text-uppercase white--text">
              <span class="font-weight-light"> Sahs</span>
              0.0.1
            </v-toolbar-title>
          </v-app-bar>
          <form class="px-16 pt-6">
            <v-text-field
              type="email"
              v-model="email"
              label="E-mail"
              @blur="checkEmail()"
              ref="email"
              required
            >
            </v-text-field>
            <v-text-field
              :append-icon="
                password_visibility ? 'visibility' : 'visibility_off'
              "
              @click:append="() => (password_visibility = !password_visibility)"
              :type="password_visibility ? 'password' : 'text'"
              v-model="password"
              label="Password"
              ref="password"
              required
            >
            </v-text-field>
            <v-row>
              <v-spacer></v-spacer>
              <v-checkbox
                v-model="check"
                label="Remember Me?"
                text
              ></v-checkbox>
              <v-spacer></v-spacer>
            </v-row>
          </form>
          <!-- <v-divider></v-divider>
          <v-container>
          <v-row>
            <v-col>
              <v-btn color="green" outlined  v-on:click="signGoogle()">
                <v-icon left>
                  mdi-google
                </v-icon>
                Sign in w/ Google?
              </v-btn>
            </v-col>
              <v-col>
              <v-btn color="blue" dark outlined v-on:click="signFacebook()">
                <v-icon left>
                  mdi-facebook
                </v-icon>
                Sign in w/ facebook?
              </v-btn>
            </v-col>
          </v-row>
        </v-container> -->

          <v-snackbar v-model="snackbar.show" :color="snackbar.state"
            >{{ snackbar.message }}
            <template v-slot:action="{ attrs }">
              <v-btn text v-bind="attrs" @click="snackbar.show = false"
                >Close
              </v-btn>
            </template>
          </v-snackbar>

          <!-- <v-alert class="mx-6" dense v-show="error.show" v-bind:state="error.state" >{{error.message}}</v-alert> -->
          <v-app-bar class="blue lighten-1 overflow hidden bottom" flat dark>
            <v-btn text @click="signup()">Signup</v-btn>
            <v-spacer></v-spacer>
            <v-btn text @click="forgot()">Forgot</v-btn>
            <v-spacer></v-spacer>
            <v-btn text @click="submit()">submit</v-btn>
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
    if (localStorage.getItem("check") == "true") this.check = true;
    else this.check = false;
    this.email = localStorage.getItem("email");
    // this.password = localStorage.getItem("password");

    if (firebase.auth().currentUser) this.$router.push("/dashboard");
  },
  mounted() {
    if (this.$refs) {
      this.$refs.email.focus();
    }
  },
  data() {
    return {
      email: "",
      password: "",
      password_visibility: true,
      check: false,
      snackbar: {
        message: "",
        state: "success",
        show: false,
      },
    };
  },
  methods: {
    signGoogle: function () {
      // var provider = new firebase.auth().GoogleAuthProvider();
      // firebase.auth().signInWithRedirect(provider);

      // firebase.auth().getRedirectResult().then(function(result) {
      //   if (result.credential) {
      //     // This gives you a Google Access Token. You can use it to access the Google API.
      //     var token = result.credential.accessToken;
      //     console.log("toker", token);
      //   }
      //   // The signed-in user info.
      //   // var user = result.user;
      //    console.log("user", result.user);

      // }).catch(function(error) {
      this.snackbar.message = "Google signin is not yet ready!";
      this.snackbar.state = "error";
      this.snackbar.show = true;
    },
    signFacebook() {
      this.snackbar.message = "Facebook signin is not yet ready!";
      this.snackbar.state = "error";
      this.snackbar.show = true;
    },
    signup() {
      this.$router.push("/signup");
    },
    submit: function () {
      if (!this.checkEmail()) return;
      if (!this.checkPassword()) return;

      if (this.check) {
        localStorage.setItem("check", this.check);
        localStorage.setItem("email", this.email);
        // localStorage.setItem("password", this.password);
      } else {
        localStorage.setItem("check", this.check);
        localStorage.setItem("email", "");
        // localStorage.setItem("password", '');
      }

      firebase
        .auth()
        .signInWithEmailAndPassword(this.email, this.password)
        .then(
          () => {
            if (!firebase.auth().currentUser.emailVerified) {
              this.snackbar.message = "Email is not verified!";
              this.snackbar.state = "warning";
              this.snackbar.show = true;
              return;
            }
            this.$router.push("/dashboard");
          },
          (error) => {
            this.snackbar.message = error;
            this.snackbar.state = "error";
            this.snackbar.show = true;
          }
        );
    },
    forgot: function () {
      if (!this.checkEmail()) return;
      firebase
        .auth()
        .sendPasswordResetEmail(this.email)
        .then(
          () => {
            this.snackbar.message =
              "Password reset has been sent to your email!";
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
    checkPassword: function () {
      if (this.password.length == 0) {
        this.snackbar.message = "Password could not be empty!";
        this.snackbar.state = "warning";
        this.snackbar.show = true;
        this.$refs.password.focus();
        return false;
      }
      if (this.password.length < 6) {
        this.snackbar.message = "Password is less than 6 letters!";
        this.snackbar.state = "warning";
        this.snackbar.show = true;
        this.$refs.password.focus();
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

        this.$refs.email.focus();
        return false;
      }

      this.snackbar.show = false;
      return true;
    },
  },
};
</script>