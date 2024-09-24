#include <iostream>
#include <vector>

#include <arrow/api.h>
#include <arrow/io/api.h>
#include <arrow/ipc/api.h>
#include <parquet/arrow/reader.h>
#include <parquet/arrow/schema.h>
#include "libKriging/utils/lk_armadillo.hpp"
#include <libKriging/Kriging.hpp>
#include <libKriging/KrigingLoader.hpp>
#include <libKriging/Trend.hpp>


std::vector<double> column2vec(const std::shared_ptr<arrow::Table> &table, const std::string &column_name) {
    std::shared_ptr<arrow::ChunkedArray> column = table->GetColumnByName(column_name);
    if (!column) {
        throw std::runtime_error("Column '" + column_name + "' not found");
    }
    // Assuming the column is of type DoubleArray (for float64 types)
    auto array = std::static_pointer_cast<arrow::DoubleArray>(column->chunk(0));
    std::vector<double> values(array->length());
    for (int64_t i = 0; i < array->length(); ++i) {
        values[i] = array->Value(i);
    }

    return values;
}

arma::mat create_input_mat(const std::vector<double> fx, const std::vector<double> fy, const std::vector<double> fz, const std::vector<double> wave_elev) {
    size_t n = wave_elev.size(); // assuming all vectors are the same length
    arma::mat input_mat(n, 4);
    for (size_t i = 0; i < n; ++i) {
        input_mat(i, 0) = fx[i];
        input_mat(i, 1) = fy[i];
        input_mat(i, 2) = fz[i];
        input_mat(i, 3) = wave_elev[i];
    }

    return input_mat;
}

arma::mat create_output_mat(const std::vector<double> px, const std::vector<double> py, const std::vector<double> pz, const std::vector<double> vx, const std::vector<double> vy, const std::vector<double> vz) {
    size_t n = px.size(); // assuming all vectors are the same length
    arma::mat output_mat(n, 6);
    for (size_t i = 0; i < n; ++i) {
        output_mat(i, 0) = px[i];
        output_mat(i, 1) = py[i];
        output_mat(i, 2) = pz[i];
        output_mat(i, 3) = vx[i];
        output_mat(i, 4) = vy[i];
        output_mat(i, 5) = vz[i];
    }

    return output_mat;
}

int main() {
    // Read in training data from parquet file
    std::shared_ptr<arrow::io::ReadableFile> infile;
    PARQUET_ASSIGN_OR_THROW(
        infile,
        arrow::io::ReadableFile::Open("openfast_lin_results_sampling.parquet"));
    std::unique_ptr<parquet::arrow::FileReader> reader;
    PARQUET_THROW_NOT_OK(
        parquet::arrow::OpenFile(infile, arrow::default_memory_pool(), &reader));
    std::shared_ptr<arrow::Table> table;
    PARQUET_THROW_NOT_OK(reader->ReadTable(&table));

    // Extract data
    auto l1fx = column2vec(table, "l1fx");
    auto l1fy = column2vec(table, "l1fy");
    auto l1fz = column2vec(table, "l1fz");
    auto l1px = column2vec(table, "l1px");
    auto l1py = column2vec(table, "l1py");
    auto l1pz = column2vec(table, "l1pz");
    auto l1vx = column2vec(table, "l1vx");
    auto l1vy = column2vec(table, "l1vy");
    auto l1vz = column2vec(table, "l1vz");
    auto l2fx = column2vec(table, "l2fx");
    auto l2fy = column2vec(table, "l2fy");
    auto l2fz = column2vec(table, "l2fz");
    auto l2px = column2vec(table, "l2px");
    auto l2py = column2vec(table, "l2py");
    auto l2pz = column2vec(table, "l2pz");
    auto l2vx = column2vec(table, "l2vx");
    auto l2vy = column2vec(table, "l2vy");
    auto l2vz = column2vec(table, "l2vz");
    auto l3fx = column2vec(table, "l3fx");
    auto l3fy = column2vec(table, "l3fy");
    auto l3fz = column2vec(table, "l3fz");
    auto l3px = column2vec(table, "l3px");
    auto l3py = column2vec(table, "l3py");
    auto l3pz = column2vec(table, "l3pz");
    auto l3vx = column2vec(table, "l3vx");
    auto l3vy = column2vec(table, "l3vy");
    auto l3vz = column2vec(table, "l3vz");
    auto wave_elev = column2vec(table, "wave_elev");

    auto X1 = create_input_mat(l1fx, l1fy, l1fz, wave_elev);
    auto X2 = create_input_mat(l2fx, l2fy, l2fz, wave_elev);
    auto X3 = create_input_mat(l3fx, l3fy, l3fz, wave_elev);
    auto y1 = create_output_mat(l1px, l1py, l1pz, l1vx, l1vy, l1vz);
    auto y2 = create_output_mat(l2px, l2py, l2pz, l2vx, l2vy, l2vz);
    auto y3 = create_output_mat(l3px, l3py, l3pz, l3vx, l3vy, l3vz);

    Kriging model_l1 = Kriging("gauss");
    Kriging model_l2 = Kriging("gauss");
    Kriging model_l3 = Kriging("gauss");

    model_l1.fit(y1, X1);
    model_l2.fit(y2, X2);
    model_l3.fit(y3, X3);

    model_l1.save("model_l1.json");
    model_l2.save("model_l2.json");
    model_l3.save("model_l3.json");

    return 0;
}