//
//  chat.hpp
//
//  Created by MNN on 2023/03/24.
//  ZhaodeWang
//

#ifndef CHAT_hpp
#define CHAT_hpp

#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <iostream>

#include <MNN/AutoTime.hpp>
#include <MNN/expr/Expr.hpp>
#include <MNN/expr/Module.hpp>
#include <MNN/expr/MathOp.hpp>
#include <MNN/expr/NeuralNetWorkOp.hpp>

#include <sentencepiece_processor.h>

using namespace MNN;
using namespace Express;

static constexpr int MASK = 130000;
static constexpr int gMASK = 130001;
static constexpr int BOS = 130004;
static constexpr int EOS = 130005;

static constexpr int VOCAB_SIZE = 130528;
static constexpr int HIDDEN_SIZE = 4096;
static constexpr int LAYER_SIZE = 28;

class ChatGLM {
public:
    // set gpu memory size (G)
    ChatGLM(float gpu_memory = 0) {
        init(gpu_memory);
    }
    void chat();
    std::string response(const std::string& input_str, std::ostream* os = &std::cout);
private:
    void init(float gpu_memory);
    void loadModel(const char* fileName, bool cuda = false);
    std::vector<int> tokenizer_encode(const std::string &input_str);
    std::string decode(int id);
    VARP gen_embedding(const std::vector<int>& input_ids);
    VARP gen_attention_mask(const std::vector<int>& input_ids);
    VARP gen_position_ids(const std::vector<int>& input_ids);
    int var_to_token(VARP var);
    int forward(const std::vector<int>& input_ids);
private:
    sentencepiece::SentencePieceProcessor sp_processor;
    // MNN Modules
    std::shared_ptr<Executor::RuntimeManager> mCPURtmgr;
    std::shared_ptr<Executor::RuntimeManager> mGPURtmgr;
    std::vector<std::shared_ptr<Module>> mModules;
    std::vector<VARP> mHistoryVars;
    // mask info
    int mSeqLen = 0, mContextLen = -1, mMaskIdx = -1;
};

#endif // CHAT_hpp