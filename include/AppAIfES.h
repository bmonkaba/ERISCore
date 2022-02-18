/**
 * @file AppAIfES.h
 * @author Brian Monkaba (brian.monkaba@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "AppBaseClass.h"
#include <aifes.h>


/**
 * @brief initalizes and trains a neural net
 * implements an AIfES example within the context of the Eris Core class framework
 */
class AppAIfES:public AppBaseClass {
  public:
    AppAIfES():AppBaseClass(){
        strcpy(name,"AI");
        x_test = NULL;
        y_test = NULL;
        // The layer structures for F32 data type and their configurations
        input_layer_shape[0] = 1;
        input_layer_shape[1] = 3;
        input_layer      = AILAYER_INPUT_F32_A(2, input_layer_shape);
        dense_layer_1    = AILAYER_DENSE_F32_A(3);
        leaky_relu_layer = AILAYER_LEAKY_RELU_F32_A(0.01f);
        dense_layer_2    = AILAYER_DENSE_F32_A(2);
        sigmoid_layer    = AILAYER_SIGMOID_F32_A();

        model.input_layer = ailayer_input_f32_default(&input_layer);
        x_layer = ailayer_dense_f32_default(&dense_layer_1, model.input_layer);
        x_layer = ailayer_leaky_relu_f32_default(&leaky_relu_layer, x_layer);
        x_layer = ailayer_dense_f32_default(&dense_layer_2, x_layer);
        x_layer = ailayer_sigmoid_f32_default(&sigmoid_layer, x_layer);
        model.output_layer = x_layer;
        
        // Finish the model creation by checking the connections and setting some parameters for further processing
        aialgo_compile_model(&model);

        uint32_t parameter_memory_size = aialgo_sizeof_parameter_memory(&model);
        parameter_memory = extmem_malloc(parameter_memory_size);
 
        // Distribute the memory to the trainable parameters of the model
        aialgo_distribute_parameter_memory(&model, parameter_memory, parameter_memory_size);
        //aialgo_print_model_structure(&model);
                
        model.loss = ailoss_crossentropy_f32_default(&crossentropy_loss, model.output_layer);
        //aialgo_print_loss_specs(model.loss);

        //config the optimizer
        adam_opti = AIOPTI_ADAM_F32(0.01f, 0.9f, 0.999f, 1e-7f);
        optimizer = aiopti_adam_f32_default(&adam_opti);
        //aialgo_print_optimizer_specs(optimizer);

        // Set the seed for your configured random function for example with
        srand(time(0));
        
        aimath_f32_default_init_he_uniform(&dense_layer_1.weights);
        aimath_f32_default_init_zeros(&dense_layer_1.bias);
        
        aimath_f32_default_init_glorot_uniform(&dense_layer_2.weights);
        aimath_f32_default_init_zeros(&dense_layer_2.bias);

        uint32_t training_memory_size = aialgo_sizeof_training_memory(&model, optimizer);
        void *training_memory = extmem_malloc(training_memory_size);
        
        // Schedule the memory to the model
        aialgo_schedule_training_memory(&model, optimizer, training_memory, training_memory_size);

        
        // One epoch of training. Iterates through the whole data once
        //float loss;
        //int batch_size = 3;
        //aialgo_train_model(&model, &x_train, &y_train, optimizer, batch_size);
        //aialgo_calc_loss_model_f32(&model, x_test, y_test, &loss);
            
        // Print the loss to the console
        //aiprint("M VM Epoch "); aiprint_int("%5d", 0);
        //aiprint(": test loss: "); aiprint_float("%f", loss);
        //aiprint("\n");
  

        //extmem_free(parameter_memory);
        //extmem_free(training_memory); 
    }; 

    //define event handlers
    void messageHandler(AppBaseClass *sender, const char *message){   
        if(sender->isName("SOME APP NAME")){
         //do something
        }
    }
  protected:
    void *parameter_memory;
    uint16_t input_layer_shape[2];
    // The main model structure that holds the whole neural network
    aimodel_t model;
    ailayer_input_f32_t input_layer;
    ailayer_dense_f32_t dense_layer_1;
    ailayer_leaky_relu_f32_t leaky_relu_layer;
    ailayer_dense_f32_t dense_layer_2;
    ailayer_sigmoid_f32_t sigmoid_layer;
    // Layer pointer to perform the connection
    ailayer_t *x_layer;
    ailoss_crossentropy_f32_t crossentropy_loss;
    aiopti_adam_f32_t adam_opti = AIOPTI_ADAM_F32(0.01f, 0.9f, 0.999f, 1e-7f);
    aiopti_t *optimizer = aiopti_adam_f32_default(&adam_opti);

    aitensor_t x_train;
    aitensor_t y_train;
    aitensor_t *x_test;
    aitensor_t *y_test;

    void render() override{
        //perform the training
        uint16_t x_train_shape[2] = {3, 3};
        float x_train_data[3*3] = {0.0f, 0.0f, 0.0f,
                                1.0f, 1.0f, 1.0f,
                                1.0f, 0.0f, 0.0f};
        x_train = AITENSOR_2D_F32(x_train_shape, x_train_data);
        
        // Target data / Labels for training
        uint16_t y_train_shape[2] = {3, 2};
        float y_train_data[3*2] = {1.0f, 0.0f,
                                0.0f, 1.0f,
                                0.0f, 0.0f};
        y_train = AITENSOR_2D_F32(y_train_shape, y_train_data);

        x_test = &x_train;
        y_test = &y_train;


        // One epoch of training. Iterates through the whole data once
        float loss;
        int batch_size = 300;
        
        aialgo_train_model(&model, &x_train, &y_train, optimizer, batch_size);
        aialgo_calc_loss_model_f32(&model, x_test, y_test, &loss);
            
        // Print the loss to the console
        //aiprint("M VM Epoch "); aiprint_int("%5d", 0);
        //aiprint(": test loss: "); aiprint_float("%f", loss);
        //aiprint("\n");
    };    //called only when the app is active
    void update() override{}; //allways called even if app is not active
    void onFocus() override{};   //called when given focus
    void onFocusLost() override{}; //called when focus is taken
    void onTouch(uint16_t t_x, uint16_t t_y) override{
        //check if touch point is within the application bounding box
        if (t_x > x && t_x < (x + w) && t_y > y && t_y < (y + h)){
            //is touched
        }
    };
    void onTouchDrag(uint16_t t_x, uint16_t t_y) override{};
    void onTouchRelease(uint16_t t_x, uint16_t t_y) override{};
    void onAnalog1(uint16_t uval, float fval) override{};
    void onAnalog2(uint16_t uval, float fval) override{};
    void onAnalog3(uint16_t uval, float fval) override{};
    void onAnalog4(uint16_t uval, float fval) override{};
};