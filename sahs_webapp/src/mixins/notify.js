
export default {
    created() {

    },

    data() {

    },

    methods: {
        notifyOpen(message, status) {
            this.$store.dispatch("notifyOpen", {
                message: message,
                status: status,
                visible: true,
            });
        },
        notifyClose() {
            this.$store.dispatch("notifyClose")
        }
    }

}

