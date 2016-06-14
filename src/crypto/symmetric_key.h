// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CRYPTO_SYMMETRIC_KEY_H_
#define CRYPTO_SYMMETRIC_KEY_H_

#include <stddef.h>

#include <string>

#include "base/macros.h"
#include "build/build_config.h"
#include "crypto/crypto_export.h"

namespace crypto {

// Wraps a platform-specific symmetric key and allows it to be held in a
// scoped_ptr.
class CRYPTO_EXPORT SymmetricKey {
 public:
  // Defines the algorithm that a key will be used with. See also
  // classs Encrptor.
  enum Algorithm {
    AES,
    HMAC_SHA1,
  };

  virtual ~SymmetricKey();

  // Generates a random key suitable to be used with |algorithm| and of
  // |key_size_in_bits| bits. |key_size_in_bits| must be a multiple of 8.
  // The caller is responsible for deleting the returned SymmetricKey.
  static SymmetricKey* GenerateRandomKey(Algorithm algorithm,
                                         size_t key_size_in_bits);

  // Derives a key from the supplied password and salt using PBKDF2, suitable
  // for use with specified |algorithm|. Note |algorithm| is not the algorithm
  // used to derive the key from the password. |key_size_in_bits| must be a
  // multiple of 8. The caller is responsible for deleting the returned
  // SymmetricKey.
  static SymmetricKey* DeriveKeyFromPassword(Algorithm algorithm,
                                             const std::string& password,
                                             const std::string& salt,
                                             size_t iterations,
                                             size_t key_size_in_bits);

  // Imports an array of key bytes in |raw_key|. This key may have been
  // generated by GenerateRandomKey or DeriveKeyFromPassword and exported with
  // GetRawKey, or via another compatible method. The key must be of suitable
  // size for use with |algorithm|. The caller owns the returned SymmetricKey.
  static SymmetricKey* Import(Algorithm algorithm, const std::string& raw_key);

  const std::string& key() { return key_; }

  // Extracts the raw key from the platform specific data.
  // Warning: |raw_key| holds the raw key as bytes and thus must be handled
  // carefully.
  bool GetRawKey(std::string* raw_key);

 private:
  SymmetricKey() {}
  std::string key_;

  DISALLOW_COPY_AND_ASSIGN(SymmetricKey);
};

}  // namespace crypto

#endif  // CRYPTO_SYMMETRIC_KEY_H_
