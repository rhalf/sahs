<template>
  <v-app>
    <v-main>
      <navbar></navbar>
      <router-view></router-view>
    </v-main>
    <notify></notify>
  </v-app>
</template>

<script>
import Navbar from "@/components/Navbar";
import Notify from "@/components/Notify";

export default {
  name: "App",
  components: { Navbar, Notify }
};
</script>
