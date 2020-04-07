#include "brave/chromium_src/components/sync/driver/sync_auth_manager.h"

#include "base/base64.h"
#include "base/strings/string_number_conversions.h"
#include "base/time/time.h"
#include "brave/components/brave_sync/crypto/crypto.h"

#define BRAVE_REQUEST_ACCESS_TOKEN                                   \
  if (private_key_.empty() || public_key_.empty()) {                 \
    ScheduleAccessTokenRequest();                                    \
    return;                                                          \
  }                                                                  \
  std::string client_id, client_secret, timestamp;                   \
  GenerateClientIdAndSecret(&client_id, &client_secret, &timestamp); \
  access_token_fetcher_->Start(client_id, client_secret, timestamp);
#include "../../../../../components/sync/driver/sync_auth_manager.cc"
#undef BRAVE_REQUEST_ACCESS_TOKEN

namespace syncer {

void SyncAuthManager::CreateAccessTokenFetcher(
    scoped_refptr<network::SharedURLLoaderFactory> url_loader_factory) {
  access_token_fetcher_ = std::make_unique<brave_sync::AccessTokenFetcherImpl>(
      this, url_loader_factory, "");
}

void SyncAuthManager::DeriveSigningKeys(const std::string& seed) {
  if (seed.empty())
    return;
  const std::vector<uint8_t> HKDF_SALT = {
      72,  203, 156, 43,  64,  229, 225, 127, 214, 158, 50,  29,  130,
      186, 182, 207, 6,   108, 47,  254, 245, 71,  198, 109, 44,  108,
      32,  193, 221, 126, 119, 143, 112, 113, 87,  184, 239, 231, 230,
      234, 28,  135, 54,  42,  9,   243, 39,  30,  179, 147, 194, 211,
      212, 239, 225, 52,  192, 219, 145, 40,  95,  19,  142, 98};
  std::vector<uint8_t> seed_bytes;
  brave_sync::crypto::PassphraseToBytes32(seed, &seed_bytes);
  brave_sync::crypto::DeriveSigningKeysFromSeed(seed_bytes, &HKDF_SALT,
                                                &public_key_, &private_key_);
}

void SyncAuthManager::OnGetTokenSuccess(
    const brave_sync::AccessTokenConsumer::TokenResponse& token_response) {
  access_token_ = token_response.access_token;
  VLOG(1) << "Got Token: " << access_token_;
  SetLastAuthError(GoogleServiceAuthError::AuthErrorNone());
}
void SyncAuthManager::OnGetTokenFailure(const std::string& error) {
  LOG(ERROR) << __func__ << ": " << error;
  // TODO(darkdh): SetLastAuthError
  request_access_token_backoff_.InformOfRequest(false);
  ScheduleAccessTokenRequest();
}

void SyncAuthManager::GenerateClientIdAndSecret(std::string* client_id,
                                                std::string* client_secret,
                                                std::string* timestamp) {
  DCHECK(client_id);
  DCHECK(client_secret);

  const std::string client_id_hex =
      base::HexEncode(&public_key_, public_key_.size());
  base::Base64Encode(client_id_hex, client_id);
  DCHECK(!client_id->empty());

  double timestamp_d = base::Time::Now().ToDoubleT();
  const std::string timestamp_hex =
      base::HexEncode(&timestamp_d, sizeof(double));
  base::Base64Encode(timestamp_hex, timestamp);
  DCHECK(!timestamp->empty());

  std::vector<uint8_t> timestamp_bytes;
  base::HexStringToBytes(timestamp_hex, &timestamp_bytes);
  std::vector<uint8_t> signature;
  brave_sync::crypto::Sign(timestamp_bytes, private_key_, &signature);
  DCHECK(brave_sync::crypto::Verify(timestamp_bytes, signature, public_key_));

  const std::string signature_hex =
      base::HexEncode(&signature, signature.size());
  base::Base64Encode(signature_hex, client_secret);
  DCHECK(!client_secret->empty());

  VLOG(1) << "client_id= " << *client_id;
  VLOG(1) << "client_secret= " << *client_secret;
  VLOG(1) << "timestamp= " << *timestamp;
}

}  // namespace syncer
