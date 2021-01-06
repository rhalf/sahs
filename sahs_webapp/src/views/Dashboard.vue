<template>
  <v-container fluid>
    <v-row>
      <v-col>
        <v-slider v-model="card_size" min="200" max="1000"> </v-slider>
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text">DateTime</v-card-title>
            <v-card-subtitle class="blue white--text"
              >Locale Time</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h2>{{ (data[0].ts * 1000) | toDateTime }}</h2>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text">Temperature</v-card-title>
            <v-card-subtitle class="blue white--text">Celcius</v-card-subtitle>
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data[0].te | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text"
              >Total Dissolve Solids</v-card-title
            >
            <v-card-subtitle class="blue white--text"
              >Nephelometric Turbidity Unit(Ntu)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data[0].td | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text">Acidity</v-card-title>
            <v-card-subtitle class="blue white--text"
              >Potential of Hydrogen(Ph)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data[0].ph | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text"
              >Oxidation-Reduction Potential (Redox)</v-card-title
            >
            <v-card-subtitle class="blue white--text"
              >milliVolts (mV)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data[0].or | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text"
              >Electric Conductivity</v-card-title
            >
            <v-card-subtitle class="blue white--text"
              >Microsiemens per centimeter (μS/cm)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data[0].ec | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text">Salinity</v-card-title>
            <v-card-subtitle class="blue white--text"
              >Parts per Trillion(ppt)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data[0].sa | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text"
              >Dissolve Oxygen</v-card-title
            >
            <v-card-subtitle class="blue white--text"
              >milligrams per Liter(mg/L)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data[0].do | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <!-- ================================================================ -->
      <v-col v-for="io in ios" v-bind:key="io.id">
        <v-hover v-slot="{ hover }" v-if="data[0]">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="red white--text"
              >Output {{ io.id }} - {{ io.label }}</v-card-title
            >
            <v-card-subtitle class="red white--text">
              interval : {{ io.interval }} seconds | enabled :
              {{ io.enabled ? "yes" : "no" }}
            </v-card-subtitle>
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ io.state ? "on" : "off" }}</h1>
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

var datasRef = database.ref("/devices/0/datas");
var iosRef = database.ref("/devices/0/io");

export default {
  mixins: [notify],
  created() {
    datasRef
      .orderByChild("dt")
      .limitToLast(1)
      .once("value")
      .then((snapshot) => {
        var pObj = snapshot.val();
        Object.keys(pObj).map((key) => {
          pObj[key].id = key;
          this.$set(this.data, 0, pObj[key]);
        });
      })
      .catch((error) => this.notifyOpen(error, "error"));

    datasRef.on("child_added", (snapshot) => {
      if (!this.data) return;
      if (!this.data[0]) return;

      Object.assign(this.data[0], snapshot.val());
    });
    //=============================================
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
  data() {
    return {
      card_size: 400,
      hover: false,
      data: {},
      ios: {},
    };
  },
};
</script>

