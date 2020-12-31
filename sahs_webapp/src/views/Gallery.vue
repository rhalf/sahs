<template>
  <v-container fluid>
    <v-row>
      <v-col>
        <v-slider v-model="image_size" max="12" min="1"> </v-slider>
      </v-col>
    </v-row>

    <v-row>
      <v-col
        :cols="image_size"
        v-for="image in images.slice().reverse()"
        :key="image.src"
      >
        <v-hover v-slot="{ hover }">
          <v-card :elevation="hover ? 5 : 1" class="pa-2">
            <v-img
              :src="image.src"
              :lazy-src="image.src"
              aspect-ratio="1"
              class="grey lighten-2"
            >
              <template v-slot:placeholder>
                <v-row class="fill-height ma-0" align="center" justify="center">
                  <v-progress-circular
                    indeterminate
                    color="grey lighten-5"
                  ></v-progress-circular>
                </v-row>
              </template>
            </v-img>
          </v-card>
        </v-hover>
      </v-col>
    </v-row>
  </v-container>
</template>

<script>
// // @ is an alias to /src
// import HelloWorld from '@/components/HelloWorld.vue'

export default {
  name: "gallery",
  data() {
    return {
      hover: false,
      image_size: 3,
      expand_transition: false,
      images: [
        { src: require("../assets/gallery/1.jpg") },
        { src: require("../assets/gallery/2.jpg") },
        { src: require("../assets/gallery/3.jpg") },
        { src: require("../assets/gallery/4.jpg") },
        { src: require("../assets/gallery/6.jpg") },
        { src: require("../assets/gallery/7.jpg") },
        { src: require("../assets/gallery/8.jpg") },
        { src: require("../assets/gallery/9.jpg") },
        { src: require("../assets/gallery/10.jpg") },
        { src: require("../assets/gallery/11.jpg") },
        { src: require("../assets/gallery/12.jpg") },
        { src: require("../assets/gallery/13.jpg") },
        { src: require("../assets/gallery/14.jpg") },
        { src: require("../assets/gallery/15.jpg") },
        { src: require("../assets/gallery/16.jpg") },
      ],
    };
  },
  methods: {},
};
</script>
