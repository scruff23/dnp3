/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */
#ifndef SECAUTHV5_OAUTHPROVIDER_H
#define SECAUTHV5_OAUTHPROVIDER_H

#include "opendnp3/outstation/IOutstationAuthProvider.h"

#include "SecurityState.h"
#include "OutstationAuthSettings.h"
#include "IAuthRequestHandler.h"

#include "secauthv5/IUserDatabase.h"

#include <openpal/util/Uncopyable.h>
#include <openpal/executor/IUTCTimeSource.h>

namespace secauthv5
{

/**
	SAv5 outstation authentication provider
*/
class OutstationAuthProvider : public opendnp3::IOutstationAuthProvider, private openpal::Uncopyable, private IAuthRequestHandler
{
	public:

	OutstationAuthProvider(
		const OutstationAuthSettings& settings,
		openpal::Logger logger, 
		openpal::IExecutor& executor,
		openpal::IUTCTimeSource& timeSource,
		IUserDatabase& userDatabase,
		openpal::ICryptoProvider& crypto
	);

	virtual void Reset() override final;	

	virtual void CheckState(opendnp3::OState& ostate) override final;
		
	virtual void OnReceive(opendnp3::OState& ostate, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects) override final;

	private:

	void Process(opendnp3::OState& ostate, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

	void OnAuthRequest(opendnp3::OState& ostate, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

	void OnUnknownRequest(opendnp3::OState& ostate, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const openpal::ReadBufferView& objects);

	//// --- IAuthRequestHandler ----

	virtual void OnAuthChallenge(opendnp3::OState& ostate, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var1Def& challenge) override final;

	virtual void OnAuthReply(opendnp3::OState& ostate, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var2Def& reply) override final;

	virtual void OnRequestKeyStatus(opendnp3::OState& ostate, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var4& status) override final;

	virtual void OnChangeSessionKeys(opendnp3::OState& ostate, const openpal::ReadBufferView& fragment, const opendnp3::APDUHeader& header, const opendnp3::Group120Var6Def& change) override final;

	/// --- State ---

	SecurityState sstate;
};

}

#endif
