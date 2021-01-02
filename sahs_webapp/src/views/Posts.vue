<template>
  <v-container fluid>
    <v-row>
      <v-col>
        <v-row class="px-3 py-2">
          <v-col>
            <PostDialogAdd :method="queryAll"></PostDialogAdd>
          </v-col>
          <v-col>
            <v-btn text @click="sortBy('title', 'asc')">
              <v-icon left small>folder</v-icon>
              <span class="caption text-lowercase">by title</span>
            </v-btn>

            <v-btn text @click="sortBy('author', 'asc')">
              <v-icon left small>person</v-icon>
              <span class="caption text-lowercase">by author</span>
            </v-btn>

            <v-btn text @click="sortBy('datetime', 'des')">
              <v-icon left small>notes</v-icon>
              <span class="caption text-lowercase">by date</span>
            </v-btn>

            <v-btn text @click="sortBy('type', 'asc')">
              <v-icon left small>check</v-icon>
              <span class="caption text-lowercase">by type</span>
            </v-btn>
          </v-col>
          <v-col>
            <v-chip
              class="v-chip--active announcement white--text caption"
              v-text="`announcement`"
            ></v-chip>
            <v-chip
              class="v-chip--active article white--text caption"
              v-text="`article`"
            ></v-chip>
            <v-chip
              class="v-chip--active question white--text caption"
              v-text="`question`"
            ></v-chip>
          </v-col>
        </v-row>
      </v-col>
    </v-row>

    <v-row>
      <v-col>
        <v-card
          v-for="post in posts"
          :key="post.id"
          :class="`${post.type} pa-1 ma-1 lighten-4`"
          outlined
          flat
        >
          <v-row>
            <v-col>
              <div class="caption ">Title</div>
              <div v-text="post.title"></div>
            </v-col>
            <v-col>
              <div class="caption ">Author</div>
              <div v-text="post.author"></div>
            </v-col>
            <v-col>
              <div class="caption ">Date</div>
              <!-- <div v-text="post.datetime | toDateTime"></div> -->
              <div :text-content.prop="post.datetime | toDateTime"></div>
            </v-col>
            <!-- <v-col >
              <div class="caption ">Type</div>
              <v-chip :class="`v-chip--active ${post.type} white--text caption`" v-text="post.type"></v-chip>
            </v-col> -->
            <!-- <v-col >
              <div class="caption ">Status</div>
                <v-chip :class="`blue  white--text caption`">{{post.status}}</v-chip>
            </v-col> -->
            <v-col>
              <v-row class="py-4" justify="center">
                <PostDialogView :post="post"></PostDialogView>
                <PostDialogEdit
                  :method="queryAll"
                  :post="post"
                  :snackbar="snackbar"
                ></PostDialogEdit>
                <PostDialogRemove
                  :method="queryAll"
                  :post="post"
                  :snackbar="snackbar"
                ></PostDialogRemove>
              </v-row>
            </v-col>
          </v-row>
        </v-card>
      </v-col>
    </v-row>
    <v-snackbar v-model="snackbar.show" :color="snackbar.state"
      >{{ snackbar.message }}
      <template v-slot:action="{ attrs }">
        <v-btn text v-bind="attrs" @click="snackbar.show = false">Close </v-btn>
      </template>
    </v-snackbar>
  </v-container>
</template>



<script>
import PostDialogView from "../components/PostDialogView.vue";
import PostDialogAdd from "../components/PostDialogAdd.vue";
import PostDialogRemove from "../components/PostDialogRemove.vue";
import PostDialogEdit from "../components/PostDialogEdit.vue";
import firebase from "../plugins/firebase";

var firestore = firebase.firestore();

export default {
  name: "Dashboard",
  created() {
    this.queryAll();
  },
  components: {
    PostDialogView,
    PostDialogAdd,
    PostDialogRemove,
    PostDialogEdit,
  },
  data() {
    return {
      reveal: false,
      posts: [],
      snackbar: {
        message: "",
        state: "success",
        show: false,
      },
    };
  },
  methods: {
    sortBy(prop, direction) {
      if (direction == "asc")
        this.posts.sort((a, b) => (a[prop] < b[prop] ? -1 : 1));
      if (direction == "des")
        this.posts.sort((a, b) => (a[prop] < b[prop] ? 1 : -1));
    },
    queryAll() {
      this.posts = [];
      firestore
        .collection("posts")
        .get()
        .then((querySnapshot) => {
          querySnapshot.forEach((doc) => {
            var post = doc.data();
            post.id = doc.id;
            this.posts.push(post);
          });
        });
    },
  },
};
</script>

<style scoped>
.v-card.announcement {
  border-left: 16px solid blue !important;
  background-color: rgba(0,0,255,.15) !important;
}
.v-card.article {
  border-left: 16px solid green !important;
  background-color: rgba(0,255,0,.15) !important;
}
.v-card.question {
  border-left: 16px solid red !important;
  background-color: rgba(255,0,0,.15) !important;
}

.v-chip.announcement {
  background-color: blue !important;
}
.v-chip.article {
  background-color: green !important;
}
.v-chip.question {
  background-color: red !important;
}
</style>