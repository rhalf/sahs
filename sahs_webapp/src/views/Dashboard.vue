<template>
  <v-container fluid>
    <v-row>
      <v-col>
        <v-slider v-model="card_size" min="200" max="1000"> </v-slider>
      </v-col>
    </v-row>
    <v-row>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text">DateTime</v-card-title>
            <v-card-subtitle class="blue white--text"
              >Locale Time</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h2>{{ (data.ts * 1000) | toDateTime }}</h2>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text">Temperature</v-card-title>
            <v-card-subtitle class="blue white--text">Celcius</v-card-subtitle>
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.te | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text"
              >Total Dissolve Solids</v-card-title
            >
            <v-card-subtitle class="blue white--text"
              >Nephelometric Turbidity Unit(Ntu)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.td | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text">Acidity</v-card-title>
            <v-card-subtitle class="blue white--text"
              >Potential of Hydrogen(Ph)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.ph | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text"
              >Oxidation-Reduction Potential (Redox)</v-card-title
            >
            <v-card-subtitle class="blue white--text"
              >milliVolts (mV)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.or | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text"
              >Electric Conductivity</v-card-title
            >
            <v-card-subtitle class="blue white--text"
              >Microsiemens per centimeter (μS/cm)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.ec | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text">Salinity</v-card-title>
            <v-card-subtitle class="blue white--text"
              >Parts per Trillion(ppt)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.sa | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="blue white--text"
              >Dissolve Oxygen</v-card-title
            >
            <v-card-subtitle class="blue white--text"
              >milligrams per Liter(mg/L)</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.do | toDouble }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="red white--text"
              >Switch / Output 0</v-card-title
            >
            <v-card-subtitle class="red white--text"
              >water pump</v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.s0 }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="red white--text"
              >Switch / Output 1</v-card-title
            >
            <v-card-subtitle class="red white--text"
              >Led lamp </v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.s1 }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="red white--text"
              >Switch / Output 2</v-card-title
            >
            <v-card-subtitle class="red white--text"
              >Not yet assigned </v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.s2 }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="red white--text"
              >Switch / Output 3</v-card-title
            >
            <v-card-subtitle class="red white--text"
              >Not yet assigned </v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.s3 }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
      <v-col>
        <v-hover v-slot="{ hover }" v-if="data">
          <v-card :elevation="hover ? 5 : 1" class="pa-2" :width="card_size">
            <v-card-title class="red white--text"
              >Switch / Output 4</v-card-title
            >
            <v-card-subtitle class="red white--text"
              >Not yet assigned </v-card-subtitle
            >
            <v-divider></v-divider>
            <v-card-text align="center">
              <h1>{{ data.s4 }}</h1>
            </v-card-text>
          </v-card>
        </v-hover>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
import firebase from "../plugins/firebase";

var database = firebase.database();
var datasRef = database.ref("/devices/0/datas");

export default {
  name: "Dashboard",

  created() {
    var query = datasRef.orderByChild("dt").limitToLast(1).once("value");

    query.then(
      (snapshot) => {
        var key = Object.keys(snapshot.val());
        var data = snapshot.val();
        this.data = data[key];
      },
      function (error) {
        console.log(error.message);
      }
    );
  },
  data() {
    return {
      card_size: 400,
      hover: false,
      data: null,
    };
  },

  methods: {},
};
</script>

