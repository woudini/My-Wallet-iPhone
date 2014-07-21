/*
 * 
 * Copyright (c) 2012, Ben Reeves. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#import <UIKit/UIKit.h>
#import <AudioToolbox/AudioToolbox.h>

#import "Wallet.h"
#import "MultiAddressResponse.h"
#import "WebSocket.h"
#import "Reachability.h"
#import "TabViewController.h"
#import "ZBarSDK.h"
#import "PEPinEntryController.h"
#import "UIModalView.h"

#define SATOSHI 100000000
#define MultiaddrCacheFile @"multiaddr.cache"
#define WalletCachefile @"wallet.aes.json"
#define WebSocketURL @"wss://ws.blockchain.info/inv"
#define WebROOT @"https://blockchain.info/"
#define MULTI_ADDR_TIME 60.0f //1 Minute

@class TransactionsViewController, Wallet, UIFadeView, ReceiveCoinsViewController, AccountViewController, SendViewController, WebViewController, NewAccountView, MulitAddressResponse;

typedef enum {
    TaskGetMultiAddr,
    TaskGetWallet,
    TaskSaveWallet,
    TaskLoadUnconfirmed,
    TaskGeneratingWallet,
    TaskLoadExternalURL
} Task;

@interface AppDelegate : UIResponder <UIApplicationDelegate, WalletDelegate, WebSocketDelegate, ZBarReaderViewDelegate,PEPinEntryControllerDelegate> {
    Wallet * wallet;
    Reachability * reachability;
    
    SystemSoundID alertSoundID;
    SystemSoundID beepSoundID;
    SystemSoundID dingSoundID;
    
    NSNumberFormatter * btcFromatter;
        
    IBOutlet UIActivityIndicatorView * activity;
    IBOutlet UIFadeView * busyView;
    IBOutlet UILabel * busyLabel;
    IBOutlet UIButton * powerButton;
    
    IBOutlet TabViewcontroller * tabViewController;
    IBOutlet TabViewcontroller * newTabViewController;
    IBOutlet TabViewcontroller * oldTabViewController;

    IBOutlet TransactionsViewController * transactionsViewController;
    IBOutlet ReceiveCoinsViewController * receiveViewController;
    IBOutlet SendViewController * sendViewController;
    IBOutlet AccountViewController * accountViewController;
    
    IBOutlet UIView * welcomeView;
    IBOutlet NewAccountView * newAccountView;
    IBOutlet UIView * pairingInstructionsView;
    IBOutlet UIButton * pairLogoutButton;
    IBOutlet UIView * secondPasswordView;
    IBOutlet UITextField * secondPasswordTextField;
    
    IBOutlet UIView * mainPasswordView;
    IBOutlet UITextField * mainPasswordTextField;

    IBOutlet UIView * manualView;
    IBOutlet UITextField * manualIdentifier;
    IBOutlet UITextField * manualSharedKey;
    IBOutlet UITextField * manualPassword;

    WebViewController * webViewController;
    
    int webScoketFailures;
    int myPin;    
    int tasks;
    
    @public
    
    BOOL symbolLocal;
}

@property (strong, nonatomic) IBOutlet UIWindow *window;
@property (retain, nonatomic) Wallet * wallet;
@property (retain, strong) MulitAddressResponse * latestResponse;

@property (retain, nonatomic) Reachability * reachability;
@property(nonatomic, strong) ZBarReaderView * readerView;

@property (retain, strong) IBOutlet MyUIModalView * modalView;


-(IBAction)manualPairClicked:(id)sender;
-(IBAction)changePinClicked:(id)sender;
-(void)setAccountData:(NSString*)guid sharedKey:(NSString*)sharedKey password:(NSString*)password;

-(void)didGenerateNewWallet:(Wallet*)wallet password:(NSString*)password;

-(void)playBeepSound;
-(void)playAlertSound;

-(TabViewcontroller*)tabViewController;
-(TransactionsViewController*)transactionsViewController;

-(void)forgetWallet;
-(void)showWelcome;

-(NSString*)guid;
-(NSString*)sharedKey;
-(NSString*)password;

//Simple Modal UIVIew
-(void)showModal:(UIView*)contentView onDismiss:(void (^)())onDismiss;
-(void)closeModal;
-(IBAction)closeModalClicked:(id)sender;

-(NSString*)checksumCache;

-(void)writeWalletCacheToDisk:(NSString*)payload;

-(NSDictionary*)parseURI:(NSString*)string;

-(void)startTask:(Task)task;
-(void)finishTask;

//Wallet Delegate
-(void)didSetLatestBlock:(LatestBlock*)block;
-(void)walletDidLoad:(Wallet *)wallet;
-(void)walletFailedToDecrypt:(Wallet*)wallet;
-(void)walletJSReady;
-(void)didSubmitTransaction;

//Status timer
-(void)checkStatus;

//Display a message
- (void)standardNotify:(NSString*)message;
- (void)standardNotify:(NSString*)message delegate:(id)fdelegate;
- (void)standardNotify:(NSString*)message title:(NSString*)title delegate:(id)fdelegate;

//Write and read from file
-(BOOL)writeToFile:(NSData *)data fileName:(NSString *)fileName;
-(NSData*)readFromFileName:(NSString *)fileName;

//Request Second Password From User
-(void)getSecondPassword:(void (^)(NSString *))success error:(void (^)(NSString *))error;


-(NSString*)formatMoney:(uint64_t)value;
-(NSString*)formatMoney:(uint64_t)value localCurrency:(BOOL)fsymbolLocal;

-(void)toggleSymbol;
  
-(void)pushWebViewController:(NSString*)url;

-(void)showSendCoins;

-(void)didSubmitTransaction;

-(IBAction)modalBackgroundClicked:(id)sender;
-(IBAction)receiveCoinClicked:(UIButton *)sender;
-(IBAction)transactionsClicked:(UIButton *)sender;
-(IBAction)sendCoinsClicked:(UIButton *)sender;
-(IBAction)infoClicked:(UIButton *)sender;
-(IBAction)powerClicked:(id)sender;
-(IBAction)signupClicked:(id)sender;
-(IBAction)loginClicked:(id)sender; 
-(IBAction)scanAccountQRCodeclicked:(id)sender;
-(IBAction)secondPasswordClicked:(id)sender;
-(IBAction)mainPasswordClicked:(id)sender;
-(IBAction)refreshClicked:(id)sender;

@end

extern AppDelegate * app;