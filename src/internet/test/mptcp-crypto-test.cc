

/* client initiates connection => SYN */
typedef struct _crypto_material {
uint64_t keyClient;
uint64_t keyServer;
uint32_t expectedTokenClient;
uint32_t expectedTokenServer;
uint32_t nonceSyn;
uint32_t nonceSynAck;
uint64_t expectedHmacSynAck;
uint32_t expectedHmacAck;
uint64_t expectedIdsnClient;
uint64_t expectedIdsnServer;
} crypto_materials_t;


/**
Used to test key/token generation
**/
class MpTcpCryptoTest : public TestCase
{
public:

  MpTcpCryptoTest(crypto_materials_t t) : TestCase("MPTCP crypto test with values ..."),m_c(t)
  {
    //!
    NS_LOG_FUNCTION(this);
  }

  virtual ~MpTcpCryptoTest()
  {
      NS_LOG_FUNCTION(this);
  }

  virtual void DoRun(void)
  {
    const mptcp_crypto_t algo = MPTCP_SHA1;
    uint32_t tokenClient = 0, tokenServer = 0;
    uint64_t idsnClient = 0, idsnServer = 0;

    MpTcpSocketBase::GenerateTokenForKey( algo, m_c.keyServer, tokenServer, idsnServer);
    MpTcpSocketBase::GenerateTokenForKey( algo, m_c.keyClient, tokenClient, idsnClient);
    NS_LOG_INFO( "Client: Generated token "<< tokenClient << ". Expected "<< m_c.expectedTokenClient);
    NS_LOG_INFO( "Client: Generated idsn "<< idsnClient << ". Expected "<< m_c.expectedIdsnClient);

    NS_LOG_INFO( "Server: Generated token " << tokenServer << ". Expected "<< m_c.expectedTokenServer);
    NS_LOG_INFO( "Server: Generated idsn " << idsnServer << ". Expected "<< m_c.expectedIdsnServer);
    NS_TEST_EXPECT_MSG_EQ( m_c.expectedTokenClient, tokenClient, "Token generated does not match key (Client)");
    NS_TEST_EXPECT_MSG_EQ( m_c.expectedIdsnClient, idsnClient, "Token generated does not match key (Client)");

    NS_TEST_EXPECT_MSG_EQ( m_c.expectedTokenServer, tokenServer, "Token generated does not match key (Server)");
    NS_TEST_EXPECT_MSG_EQ( m_c.expectedIdsnServer, idsnServer, "Token generated does not match key (Server)");

  }

protected:
  crypto_materials_t m_c;


};


static class TcpOptionMpTcpTestSuite : public TestSuite
{
public:
 TcpOptionMpTcpTestSuite ()
 : TestSuite ("mptcp-crypto", UNIT)
 {

    // Notice the 'U' suffix at the end of the number . By default compiler
    // considers int as signed, thus triggering a warning
    crypto_materials_t c = {
      .keyClient = 17578475652852252522U,
      .keyServer = 4250710109353306436U,
      /*expectedTokenClient computed from SynAck key */
      .expectedTokenClient = 781076417,
      .expectedTokenServer = 109896498,
      .nonceSyn = 4179070691,
      .nonceSynAck = 786372555,
      .expectedHmacSynAck = 17675966670101668951U,
      .expectedHmacAck = 0,
      .expectedIdsnClient =2027218329290435821U,
      .expectedIdsnServer  = 14296996231892742347U
    };

    AddTestCase( new MpTcpCryptoTest(c), QUICK );


 }




} g_TcpOptionTestSuite;
