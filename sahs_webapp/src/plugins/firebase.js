import firebase from 'firebase/app';
import 'firebase/database';
import 'firebase/auth';
import 'firebase/firestore';

// For Firebase JS SDK v7.20.0 and later, measurementId is optional
const firebaseConfig = {
  apiKey: "AIzaSyBewKXuSnUThP7PoMfjERRjXNFS7EWIXC0",
  authDomain: "sahs-9d452.firebaseapp.com",
  databaseURL: "https://sahs-9d452.firebaseio.com",
  projectId: "sahs-9d452",
  storageBucket: "sahs-9d452.appspot.com",
  messagingSenderId: "1031058839268",
  appId: "1:1031058839268:web:87c96942a1ad020df23ef1",
  measurementId: "G-66PLK1KL54"
};


firebase.initializeApp(firebaseConfig);

export default firebase



