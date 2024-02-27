// Import necessary modules
const express = require("express");
const { Storage } = require("@google-cloud/storage");
const multer = require("multer");

// Initialize express app and Google Cloud Storage
const app = express();
const storage = new Storage(); 

// Set Google Cloud credentials
const path = require("path");
process.env.GOOGLE_APPLICATION_CREDENTIALS = path.join(__dirname, "cred.json");

// Initialize multer with the destination folder for uploaded files
const upload = multer({ dest: "uploads/" });

// Specify the name of your Google Cloud Storage bucket
const bucketName = "bucket-coe453-2018xxxxx";

// Route for file upload
app.post("/upload", upload.single("file"), async (req, res) => {
  try { 
    const file = req.file;
    // Check if a file is provided
    if (!file) {
      res.status(400).send("No file uploaded.");
      return;
    }

    // Upload the file to the specified bucket
    await storage.bucket(bucketName).upload(file.path, {
      destination: file.originalname,
    });

    // Send a success response
    res.status(200).send("File uploaded.");
  } catch (err) {
    console.error("Error uploading file:", err);
    res.status(500).send("Error uploading file.");
  }
});

// Route for listing all files in the bucket
app.get("/list", async (req, res) => {
  try {
    const [files] = await storage.bucket(bucketName).getFiles();
    // Send a response with the names of all files in the bucket
    res.status(200).send(files.map((file) => file.name));
  } catch (err) {
    console.error("Error listing files:", err);
    res.status(500).send("Error listing files.");
  }
});

// Route for downloading a file
app.get("/download/:filename", async (req, res) => {
  try {
    const filename = req.params.filename;
    const file = storage.bucket(bucketName).file(filename);
    const [fileExists] = await file.exists();
    // Check if the file exists
    if (!fileExists) {
      res.status(404).send("File not found.");
      return;
    }

    // Get the metadata of the file and set the Content-Type header
    const [metadata] = await file.getMetadata();
    res.setHeader("Content-Type", metadata.contentType);

    // Create a read stream for the file and pipe it to the response
    const fileReadStream = file.createReadStream();
    fileReadStream.pipe(res);
  } catch (err) {
    console.error("Error downloading file:", err);
    res.status(500).send("Error downloading file.");
  }
});

// Start the server
app.listen(4000, () => {
  console.log("Server is running on port 4000");
});