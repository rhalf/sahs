<template>
  <v-container fluid>
    <v-row>
      <v-col>
        <v-slider v-model="card_size" min="200" max="1000"> </v-slider>
      </v-col>
    </v-row>
    <v-row>
      <v-col v-for="io in ios" v-bind:key="io.id">
        <v-hover v-slot="{ hover }">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="red white--text"
              >Output ({{ io.id }})</v-card-title
            >
            <!-- <v-card-subtitle class="red white--text">
              interval : {{ io.interval }} seconds | enabled :
              {{ io.enabled ? "yes" : "no" }}
            </v-card-subtitle> -->
            <v-divider></v-divider>
            <!-- <v-card-text align="center">
              <h1>{{ io.state ? "on" : "off" }}</h1>
            </v-card-text> -->
            <v-form class="pa-2">
              <v-text-field
                label="label"
                v-model="io.label"
                outlined
              ></v-text-field>
              <v-text-field
                label="interval"
                v-model="io.interval"
                outlined
              ></v-text-field>
              <v-row>
                <v-col>
                  <v-switch
                    label="Enabled"
                    v-model="io.enabled"
                    color="green"
                  ></v-switch>
                </v-col>
                <v-col>
                  <v-switch
                    disabled
                    label="State"
                    v-model="io.state"
                    color="green"
                  ></v-switch>
                </v-col>
              </v-row>
            </v-form>
            <v-divider></v-divider>
            <v-card-text align="center">
              <v-btn class="primary" dark block @click="save(io)">Save</v-btn>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import notify from "@/mixins/notify";
import firebase from "@/plugins/firebase";

var database = firebase.database();

var iosRef = database.ref("/devices/0/io");

export default {
  mixins: [notify],
  created() {
    iosRef
      .once("value")
      .then((snapshot) => {
        snapshot.forEach((element) => {
          this.$set(this.ios, element.val().id, element.val());
        });
      })
      .catch((error) => this.notifyOpen(error, "error"));

    iosRef.on("child_changed", (snapshot) => {
      Object.assign(this.ios[snapshot.val().id], snapshot.val());
    });
  },
  methods: {
    save(io) {
      var refString = "/devices/0/io/" + io.id.toString();
      var ioRef = database.ref(refString);

      ioRef
        .set(io)
        .then(() => {
          this.notifyOpen(
            "Successfully updated properties on io (" + io.id.toString() + ")",
            "success"
          );
        })
        .catch((error) => {
          this.notifyOpen(error, "success");
        });
    },
  },
  data() {
    return {
      card_size: 400,
      hover: false,
      data: null,
      ios: {},
    };
  },
};
</script>

