import { useState } from 'react'

export default function AddExpenseModal({ group, onClose, onAdded }) {
    const [form, setForm] = useState({
        description: '',
        amount: '',
        category: '',
        date: new Date().toISOString().slice(0, 10),
    })

    const [loading, setLoading] = useState(false)
    const [error, setError] = useState('')

    const inputClass =
        'bg-[#0e0f11] border border-[#2a2d35] text-white rounded-lg px-3.5 py-2.5 text-sm focus:outline-none focus:border-emerald-400 transition-colors placeholder:text-[#5c6070] w-full'

    const handleChange = (e) => {
        setForm((prev) => ({
        ...prev,
        [e.target.name]: e.target.value,
        }))
    }

    async function handleSubmit(e) {
        e.preventDefault()

        if (!form.description || !form.amount) {
        setError('Description and amount are required')
        return
        }

        setLoading(true)

        try {
        await onAdded({
            description: form.description,
            amount: parseFloat(form.amount),
            category: form.category || null,
            date: form.date,
        })
        } catch (err) {
        setError(err.response?.data?.message || 'Failed to add expense')
        setLoading(false)
        }
    }

    return (
        <div
        className="fixed inset-0 z-50 flex items-center justify-center bg-black/60 backdrop-blur-sm p-5"
        onClick={(e) => e.target === e.currentTarget && onClose()}
        >
        <div className="w-full max-w-md rounded-2xl border border-[#363a45] bg-[#16181c] p-8 shadow-2xl">
            <div className="mb-6">
            <h2 className="font-serif text-2xl font-normal text-white">Add expense</h2>
            <p className="mt-1 text-sm text-[#9a9da8]">
                Split equally among all {group.members?.length} members
            </p>
            </div>

            <form onSubmit={handleSubmit} className="flex flex-col gap-4">
            <div className="flex flex-col gap-1.5">
                <label className="text-xs font-medium uppercase tracking-wider text-[#9a9da8]">
                Description
                </label>

                <input
                name="description"
                placeholder="e.g. Hotel booking"
                value={form.description}
                onChange={handleChange}
                className={inputClass}
                />
            </div>

            <div className="grid grid-cols-2 gap-3">
                <div className="flex flex-col gap-1.5">
                <label className="text-xs font-medium uppercase tracking-wider text-[#9a9da8]">
                    Amount (₹)
                </label>

                <input
                    name="amount"
                    type="number"
                    min="0"
                    step="0.01"
                    placeholder="0.00"
                    value={form.amount}
                    onChange={handleChange}
                    className={inputClass}
                />
                </div>

                <div className="flex flex-col gap-1.5">
                <label className="text-xs font-medium uppercase tracking-wider text-[#9a9da8]">
                    Category
                </label>

                <select
                    name="category"
                    value={form.category}
                    onChange={handleChange}
                    className={inputClass}
                >
                    <option value="">Select...</option>
                    <option>Food</option>
                    <option>Travel</option>
                    <option>Stay</option>
                    <option>Activities</option>
                    <option>Other</option>
                </select>
                </div>
            </div>

            <div className="flex flex-col gap-1.5">
                <label className="text-xs font-medium uppercase tracking-wider text-[#9a9da8]">
                Date
                </label>

                <input
                name="date"
                type="date"
                value={form.date}
                onChange={handleChange}
                className={inputClass}
                />
            </div>

            {error && (
                <div className="rounded-lg border border-red-400/20 bg-red-400/10 px-3.5 py-2.5 text-sm text-red-400">
                {error}
                </div>
            )}

            <div className="mt-1 flex justify-end gap-2.5">
                <button
                type="button"
                onClick={onClose}
                className="rounded-lg border border-[#2a2d35] px-4 py-2 text-sm text-[#9a9da8] transition-all hover:bg-[#1e2026] hover:text-white"
                >
                Cancel
                </button>

                <button
                type="submit"
                disabled={loading}
                className="rounded-lg bg-emerald-400 px-4 py-2 text-sm font-semibold text-[#0a1a13] transition-all hover:bg-emerald-300 disabled:opacity-40"
                >
                {loading ? 'Adding...' : 'Add expense'}
                </button>
            </div>
            </form>
        </div>
        </div>
    )
}