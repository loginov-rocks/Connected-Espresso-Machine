# Deployment

## AWS CloudFormation

1. Use the CloudFormation template to deploy the stack and create a user for the GitHub CD workflow.
2. Create an access key for the user to configure GitHub Actions Secrets.

## GitHub Actions Secrets

Configure the following secrets:

1. `AWS_ACCESS_KEY_ID`
2. `AWS_SECRET_ACCESS_KEY`
3. `AWS_REGION`
4. `AWS_S3_BUCKET_NAME`
