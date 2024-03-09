const express = require('express');
const app = express();

app.get('/', (req, res) => {
    res.send('Hello World, from gcloud GCP Cloud Run');
    });

app.listen(8080, () => {
    console.log('Server is up on 8080');
    }
);