# AWS

Create an S3 bucket with public access (remove all checkboxes in the "Block Public Access settings for this bucket"
section) and configure policies as below.

## Bucket Policy

```json
{
  "Version": "2012-10-17",
  "Statement": [
    {
      "Sid": "Statement1",
      "Effect": "Allow",
      "Principal": "*",
      "Action": "s3:GetObject",
      "Resource": "arn:aws:s3:::{BUCKET_NAME}/*"
    }
  ]
}
```

## CORS Policy

```json
[
  {
    "AllowedHeaders": [],
    "AllowedMethods": [
      "GET"
    ],
    "AllowedOrigins": [
      "*"
    ],
    "ExposeHeaders": []
  }
]
```
