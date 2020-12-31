import Vue from 'vue';


Vue.filter('toDateTime', timestamp => {
    return new Date(timestamp).toLocaleString();
});

Vue.filter('toDouble', value => {
    return  value / 100;
});

export default Vue