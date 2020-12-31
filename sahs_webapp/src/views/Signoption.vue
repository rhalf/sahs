<template>
  <v-container fluid fill-height class="grey lighten-3">
    <v-row align="center" justify="center">
      <v-spacer></v-spacer>
      <v-col>
        <v-card width="500">
          <v-app-bar class="blue lighten-1 overflow hidden" flat dark>
            <v-icon>person</v-icon>
            Sahs (Smart Aquaphonics and Hydrophonics System)
          </v-app-bar>

          <v-container px-12 py-8>
            <v-row>
              <v-col>
                <v-btn block color="green" outlined>
                  <v-icon left> mdi-google </v-icon>
                  Sign in with Google?
                </v-btn>
              </v-col>
            </v-row>
            <v-row>
              <v-col>
                <v-btn block color="red" dark outlined>
                  <v-icon left> mdi-mail </v-icon>
                  Sign in with mail?
                </v-btn>
              </v-col>
            </v-row>
          </v-container>

          <v-alert
            class="mx-6"
            dense
            v-show="error.show"
            v-bind:type="error.type"
            >{{ error.message }}</v-alert
          >
          <!-- <v-app-bar class="blue lighten-1 overflow hidden bottom" flat dark> 
            <v-btn text @click="signup()">Signup</v-btn>
            <v-spacer></v-spacer>
            <v-btn text @click="forgot()">Forgot</v-btn>
            <v-spacer></v-spacer>
            <v-btn text @click="submit()">submit</v-btn>
          </v-app-bar> -->
        </v-card>
      </v-col>
      <v-spacer></v-spacer>
    </v-row>
    <v-row></v-row>
  </v-container>
</template>

<script>
import firebase from "../plugins/firebase";

export default {
  mounted() {
    if (localStorage.getItem("check") == "true") this.check = true;
    else this.check = false;
    this.email = localStorage.getItem("email");
    // this.password = localStorage.getItem("password");
  },
  updated() {},
  data() {
    return {
      email: "",
      password: "",
      check: false,
      error: {
        message: "",
        state: "success",
        show: false,
      },
    };
  },
  methods: {
    signup: function () {
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
              this.error.message = "Email is not verified!";
              this.error.type = "warning";
              this.error.show = true;
              return;
            }
            this.$router.push("/dashboard");
          },
          (error) => {
            this.error.message = error;
            this.error.type = "error";
            this.error.show = true;
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
            this.error.message = "Password reset has been sent to your email!";
            this.error.type = "success";
            this.error.show = true;
          },
          (error) => {
            this.error.message = error;
            this.error.type = "error";
            this.error.show = true;
          }
        );
    },
    checkPassword: function () {
      if (this.password.length == 0) {
        this.error.message = "Password could not be empty!";
        this.error.type = "warning";
        this.error.show = true;
        return false;
      }
      if (this.password.length < 6) {
        this.error.message = "Password is less than 6 letters!";
        this.error.type = "warning";
        this.error.show = true;
        return false;
      }
      this.error.show = false;
      return true;
    },
    checkEmail: function () {
      var re = /\S+@\S+\.\S+/;
      if (!re.test(this.email)) {
        this.error.message = "Not a valid email!";
        this.error.type = "warning";
        this.error.show = true;
        return false;
      }

      this.error.show = false;
      return true;
    },
  },
};
</script>